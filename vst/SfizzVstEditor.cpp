// SPDX-License-Identifier: BSD-2-Clause

// This code is part of the sfizz library and is licensed under a BSD 2-clause
// license. You should have receive a LICENSE.md file along with the code.
// If not, contact the sfizz maintainers at https://github.com/sfztools/sfizz

#include "SfizzVstEditor.h"
#include "SfizzVstState.h"
#include "SfizzFileScan.h"
#include "editor/Editor.h"
#include "editor/EditIds.h"
#if !defined(__APPLE__) && !defined(_WIN32)
#include "X11RunLoop.h"
#endif

using namespace VSTGUI;

static ViewRect sfizzUiViewRect { 0, 0, Editor::viewWidth, Editor::viewHeight };

enum {
    kOscTempSize = 8192,
    kOscQueueSize = 65536,
};

SfizzVstEditor::SfizzVstEditor(SfizzVstController* controller)
    : VSTGUIEditor(controller, &sfizzUiViewRect),
      oscTemp_(new uint8_t[kOscTempSize])
{
}

SfizzVstEditor::~SfizzVstEditor()
{
}

bool PLUGIN_API SfizzVstEditor::open(void* parent, const VSTGUI::PlatformType& platformType)
{
    fprintf(stderr, "[sfizz] about to open view with parent %p\n", parent);

    CRect wsize(0, 0, sfizzUiViewRect.getWidth(), sfizzUiViewRect.getHeight());
    CFrame *frame = new CFrame(wsize, this);
    this->frame = frame;

    IPlatformFrameConfig* config = nullptr;

#if !defined(__APPLE__) && !defined(_WIN32)
    X11::FrameConfig x11config;
    if (!_runLoop)
        _runLoop = new RunLoop(plugFrame);
    x11config.runLoop = _runLoop;
    config = &x11config;
#endif

    Editor* editor = editor_.get();
    if (!editor) {
        editor = new Editor(*this);
        editor_.reset(editor);
    }

    withStateLock([this]() {
        mustRedisplayState_ = true;
        mustRedisplayUiState_ = true;
        mustRedisplayPlayState_ = true;
        OscByteVec* queue = new OscByteVec;
        oscQueue_.reset(queue);
        queue->reserve(kOscQueueSize);
    });

    updateStateDisplay();

    if (!frame->open(parent, platformType, config)) {
        fprintf(stderr, "[sfizz] error opening frame\n");
        return false;
    }

    editor->open(*frame);

    absl::optional<fs::path> userFilesDir = SfizzPaths::getSfzConfigDefaultPath();
    uiReceiveValue(EditId::CanEditUserFilesDir, 1);
    uiReceiveValue(EditId::UserFilesDir, userFilesDir.value_or(fs::path()).u8string());

    return true;
}

void PLUGIN_API SfizzVstEditor::close()
{
    CFrame *frame = this->frame;
    if (frame) {
        if (editor_)
            editor_->close();
        if (frame->getNbReference() != 1)
            frame->forget();
        else
            frame->close();
        this->frame = nullptr;
    }

    withStateLock([this]() {
        oscQueue_.reset();
    });
}

///
CMessageResult SfizzVstEditor::notify(CBaseObject* sender, const char* message)
{
    CMessageResult result = VSTGUIEditor::notify(sender, message);

    if (result != kMessageNotified)
        return result;

#if !defined(__APPLE__) && !defined(_WIN32)
    if (message == CVSTGUITimer::kMsgTimer) {
        SharedPointer<VSTGUI::RunLoop> runLoop = RunLoop::get();
        if (runLoop) {
            // note(jpc) I don't find a reliable way to check if the host
            //   notifier of X11 events is working. If there is, remove this and
            //   avoid polluting Linux hosts which implement the loop correctly.
            runLoop->processSomeEvents();

            runLoop->cleanupDeadHandlers();
        }
    }
#endif

    if (message == CVSTGUITimer::kMsgTimer) {
        processOscQueue();
        updateStateDisplay();
    }

    return result;
}

void SfizzVstEditor::updateState(const SfizzVstState& state)
{
    withStateLock([this, &state]() {
        state_ = state;
        mustRedisplayState_ = true;
    });
}

void SfizzVstEditor::updateUiState(const SfizzUiState& uiState)
{
    withStateLock([this, &uiState]() {
        uiState_ = uiState;
        mustRedisplayUiState_ = true;
    });
}

void SfizzVstEditor::updatePlayState(const SfizzPlayState& playState)
{
    withStateLock([this, &playState]() {
        playState_ = playState;
        mustRedisplayPlayState_ = true;
    });
}

SfizzUiState SfizzVstEditor::getCurrentUiState() const
{
    SfizzUiState uiState;
    withStateLock([this, &uiState]() {
        uiState = uiState_;
    });
    return uiState;
}

void SfizzVstEditor::receiveMessage(const void* data, uint32_t size)
{
    // Note: may be called from non-UI thread (Reaper)

    withStateLock([this, data, size]() {
        if (OscByteVec* queue = oscQueue_.get()) {
            const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data);
            std::copy(bytes, bytes + size, std::back_inserter(*queue));
        }
    });
}

void SfizzVstEditor::processOscQueue()
{
    withStateLock([this]() {
        OscByteVec* queue = oscQueue_.get();
        if (!queue)
            return;

        const uint8_t* oscData = queue->data();
        size_t oscSize = queue->size();

        const char* path;
        const char* sig;
        const sfizz_arg_t* args;
        uint8_t buffer[1024];

        uint32_t msgSize;
        while ((msgSize = sfizz_extract_message(oscData, oscSize, buffer, sizeof(buffer), &path, &sig, &args)) > 0) {
            uiReceiveMessage(path, sig, args);
            oscData += msgSize;
            oscSize -= msgSize;
        }

        queue->clear();
    });
}

///
void SfizzVstEditor::uiSendValue(EditId id, const EditValue& v)
{
    if (id == EditId::SfzFile)
        loadSfzFile(v.to_string());
    else if (id == EditId::ScalaFile)
        loadScalaFile(v.to_string());
    else {
        SfizzVstController* ctrl = getController();

        auto normalizeAndSet = [ctrl](Vst::ParamID pid, const SfizzParameterRange& range, float value) {
            float normValue = range.normalize(value);
            ctrl->setParamNormalized(pid, normValue);
            ctrl->performEdit(pid, normValue);
        };

        switch (id) {
        case EditId::Volume:
            normalizeAndSet(kPidVolume, kParamVolumeRange, v.to_float());
            break;
        case EditId::Polyphony:
            normalizeAndSet(kPidNumVoices, kParamNumVoicesRange, v.to_float());
            break;
        case EditId::Oversampling:
            {
                const int32 value = static_cast<int32>(v.to_float());

                int32 log2Value = 0;
                for (int32 f = value; f > 1; f /= 2)
                    ++log2Value;

                normalizeAndSet(kPidOversampling, kParamOversamplingRange, log2Value);
            }
            break;
        case EditId::PreloadSize:
            normalizeAndSet(kPidPreloadSize, kParamPreloadSizeRange, v.to_float());
            break;
        case EditId::ScalaRootKey:
            normalizeAndSet(kPidScalaRootKey, kParamScalaRootKeyRange, v.to_float());
            break;
        case EditId::TuningFrequency:
            normalizeAndSet(kPidTuningFrequency, kParamTuningFrequencyRange, v.to_float());
            break;
        case EditId::StretchTuning:
            normalizeAndSet(kPidStretchedTuning, kParamStretchedTuningRange, v.to_float());
            break;

        case EditId::UserFilesDir:
            SfizzPaths::setSfzConfigDefaultPath(fs::u8path(v.to_string()));
            break;

        case EditId::UIActivePanel:
            uiState_.activePanel = static_cast<int32>(v.to_float());
            break;

        default:
            break;
        }
    }
}

void SfizzVstEditor::uiBeginSend(EditId id)
{
    Vst::ParamID pid = parameterOfEditId(id);
    if (pid != -1)
        getController()->beginEdit(pid);
}

void SfizzVstEditor::uiEndSend(EditId id)
{
    Vst::ParamID pid = parameterOfEditId(id);
    if (pid != -1)
        getController()->endEdit(pid);
}

void SfizzVstEditor::uiSendMIDI(const uint8_t* data, uint32_t len)
{
    SfizzVstController* ctl = getController();

    Steinberg::OPtr<Vst::IMessage> msg { ctl->allocateMessage() };
    if (!msg) {
        fprintf(stderr, "[Sfizz] UI could not allocate message\n");
        return;
    }

    msg->setMessageID("MidiMessage");
    Vst::IAttributeList* attr = msg->getAttributes();
    attr->setBinary("Data", data, len);
    ctl->sendMessage(msg);
}

void SfizzVstEditor::uiSendMessage(const char* path, const char* sig, const sfizz_arg_t* args)
{
    SfizzVstController* ctl = getController();

    Steinberg::OPtr<Vst::IMessage> msg { ctl->allocateMessage() };
    if (!msg) {
        fprintf(stderr, "[Sfizz] UI could not allocate message\n");
        return;
    }

    uint8_t* oscTemp = oscTemp_.get();
    uint32_t oscSize = sfizz_prepare_message(oscTemp, kOscTempSize, path, sig, args);
    if (oscSize <= kOscTempSize) {
        msg->setMessageID("OscMessage");
        Vst::IAttributeList* attr = msg->getAttributes();
        attr->setBinary("Data", oscTemp, oscSize);
        ctl->sendMessage(msg);
    }
}

///
void SfizzVstEditor::loadSfzFile(const std::string& filePath)
{
    SfizzVstController* ctl = getController();

    Steinberg::OPtr<Vst::IMessage> msg { ctl->allocateMessage() };
    if (!msg) {
        fprintf(stderr, "[Sfizz] UI could not allocate message\n");
        return;
    }

    msg->setMessageID("LoadSfz");
    Vst::IAttributeList* attr = msg->getAttributes();
    attr->setBinary("File", filePath.data(), filePath.size());
    ctl->sendMessage(msg);
}

void SfizzVstEditor::loadScalaFile(const std::string& filePath)
{
    SfizzVstController* ctl = getController();

    Steinberg::OPtr<Vst::IMessage> msg { ctl->allocateMessage() };
    if (!msg) {
        fprintf(stderr, "[Sfizz] UI could not allocate message\n");
        return;
    }

    msg->setMessageID("LoadScala");
    Vst::IAttributeList* attr = msg->getAttributes();
    attr->setBinary("File", filePath.data(), filePath.size());
    ctl->sendMessage(msg);
}

void SfizzVstEditor::updateStateDisplay()
{
    if (!frame)
        return;

    withStateLock([this]() {
        if (mustRedisplayState_) {
            uiReceiveValue(EditId::SfzFile, state_.sfzFile);
            uiReceiveValue(EditId::Volume, state_.volume);
            uiReceiveValue(EditId::Polyphony, state_.numVoices);
            uiReceiveValue(EditId::Oversampling, 1u << state_.oversamplingLog2);
            uiReceiveValue(EditId::PreloadSize, state_.preloadSize);
            uiReceiveValue(EditId::ScalaFile, state_.scalaFile);
            uiReceiveValue(EditId::ScalaRootKey, state_.scalaRootKey);
            uiReceiveValue(EditId::TuningFrequency, state_.tuningFrequency);
            uiReceiveValue(EditId::StretchTuning, state_.stretchedTuning);
            mustRedisplayState_ = false;
        }

        ///
        if (mustRedisplayUiState_) {
            uiReceiveValue(EditId::UIActivePanel, uiState_.activePanel);
            mustRedisplayUiState_ = false;
        }

        ///
        if (mustRedisplayPlayState_) {
            uiReceiveValue(EditId::UINumCurves, playState_.curves);
            uiReceiveValue(EditId::UINumMasters, playState_.masters);
            uiReceiveValue(EditId::UINumGroups, playState_.groups);
            uiReceiveValue(EditId::UINumRegions, playState_.regions);
            uiReceiveValue(EditId::UINumPreloadedSamples, playState_.preloadedSamples);
            uiReceiveValue(EditId::UINumActiveVoices, playState_.activeVoices);
            mustRedisplayPlayState_ = false;
        }
    });
}

Vst::ParamID SfizzVstEditor::parameterOfEditId(EditId id)
{
    switch (id) {
    case EditId::Volume: return kPidVolume;
    case EditId::Polyphony: return kPidNumVoices;
    case EditId::Oversampling: return kPidOversampling;
    case EditId::PreloadSize: return kPidPreloadSize;
    case EditId::ScalaRootKey: return kPidScalaRootKey;
    case EditId::TuningFrequency: return kPidTuningFrequency;
    case EditId::StretchTuning: return kPidStretchedTuning;
    default: return -1;
    }
}
