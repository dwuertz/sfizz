include(GNUInstallDirs)

set(FAUST_FILES
    sfizz/dsp/filters/filters_modulable.dsp
    sfizz/dsp/filters/rbj_filters.dsp
    sfizz/dsp/filters/sallenkey_modulable.dsp
    sfizz/dsp/filters/sfz_filters.dsp
    sfizz/effects/dsp/limiter.dsp
    sfizz/effects/dsp/resonant_string.dsp
    sfizz/effects/dsp/compressor.dsp
    sfizz/effects/dsp/gate.dsp
    sfizz/effects/dsp/disto_stage.dsp
    sfizz/effects/dsp/fverb.dsp)
source_group("Faust Files" FILES ${FAUST_FILES})

set(SFIZZ_HEADERS
    sfizz/ADSREnvelope.h
    sfizz/AudioBuffer.h
    sfizz/AudioReader.h
    sfizz/AudioSpan.h
    sfizz/BeatClock.h
    sfizz/Buffer.h
    sfizz/BufferPool.h
    sfizz/CCMap.h
    sfizz/Config.h
    sfizz/Curve.h
    sfizz/Debug.h
    sfizz/utility/NumericId.h
    sfizz/utility/SpinMutex.h
    sfizz/utility/XmlHelpers.h
    sfizz/modulations/ModId.h
    sfizz/modulations/ModKey.h
    sfizz/modulations/ModKeyHash.h
    sfizz/modulations/ModMatrix.h
    sfizz/modulations/ModGenerator.h
    sfizz/modulations/sources/ADSREnvelope.h
    sfizz/modulations/sources/Controller.h
    sfizz/modulations/sources/FlexEnvelope.h
    sfizz/modulations/sources/LFO.h
    sfizz/effects/impl/ResonantArray.h
    sfizz/effects/impl/ResonantArrayAVX.h
    sfizz/effects/impl/ResonantArraySSE.h
    sfizz/effects/impl/ResonantString.h
    sfizz/effects/impl/ResonantStringAVX.h
    sfizz/effects/impl/ResonantStringSSE.h
    sfizz/effects/Apan.h
    sfizz/effects/CommonLFO.h
    sfizz/effects/CommonLFO.hpp
    sfizz/effects/Compressor.h
    sfizz/effects/Disto.h
    sfizz/effects/Eq.h
    sfizz/effects/Filter.h
    sfizz/effects/Fverb.h
    sfizz/effects/Gain.h
    sfizz/effects/Gate.h
    sfizz/effects/Limiter.h
    sfizz/effects/Lofi.h
    sfizz/effects/Nothing.h
    sfizz/effects/Rectify.h
    sfizz/effects/Strings.h
    sfizz/effects/Width.h
    sfizz/Effects.h
    sfizz/EGDescription.h
    sfizz/EQDescription.h
    sfizz/EQPool.h
    sfizz/FileId.h
    sfizz/FileMetadata.h
    sfizz/FilePool.h
    sfizz/FilterDescription.h
    sfizz/FilterPool.h
    sfizz/FlexEGDescription.h
    sfizz/FlexEnvelope.h
    sfizz/HistoricalBuffer.h
    sfizz/Interpolators.h
    sfizz/Interpolators.hpp
    sfizz/Logger.h
    sfizz/LFO.h
    sfizz/LFODescription.h
    sfizz/MathHelpers.h
    sfizz/Metronome.h
    sfizz/MidiState.h
    sfizz/ModifierHelpers.h
    sfizz/OnePoleFilter.h
    sfizz/Oversampler.h
    sfizz/Panning.h
    sfizz/PolyphonyGroup.h
    sfizz/PowerFollower.h
    sfizz/railsback/2-1.h
    sfizz/railsback/4-1.h
    sfizz/railsback/4-2.h
    sfizz/Region.h
    sfizz/RegionSet.h
    sfizz/Resources.h
    sfizz/RTSemaphore.h
    sfizz/ScopedFTZ.h
    sfizz/SfzFilter.h
    sfizz/SfzFilterImpls.hpp
    sfizz/simd/Common.h
    sfizz/simd/HelpersAVX.h
    sfizz/simd/HelpersScalar.h
    sfizz/simd/HelpersSSE.h
    sfizz/SIMDConfig.h
    sfizz/SIMDHelpers.h
    sfizz/SisterVoiceRing.h
    sfizz/Smoothers.h
    sfizz/SwapAndPop.h
    sfizz/Synth.h
    sfizz/SynthConfig.h
    sfizz/SynthPrivate.h
    sfizz/Tuning.h
    sfizz/Voice.h
    sfizz/VoiceManager.h
    sfizz/VoiceStealing.h
    sfizz/Wavetables.h
    sfizz/WindowedSinc.h
    sfizz/WindowedSinc.hpp
    sfizz.h
    sfizz.hpp)

set(SFIZZ_SOURCES
    sfizz/Synth.cpp
    sfizz/FileId.cpp
    sfizz/FilePool.cpp
    sfizz/FileMetadata.cpp
    sfizz/AudioReader.cpp
    sfizz/FilterPool.cpp
    sfizz/EQPool.cpp
    sfizz/Region.cpp
    sfizz/Voice.cpp
    sfizz/ScopedFTZ.cpp
    sfizz/MidiState.cpp
    sfizz/SfzHelpers.cpp
    sfizz/Oversampler.cpp
    sfizz/ADSREnvelope.cpp
    sfizz/Logger.cpp
    sfizz/SfzFilter.cpp
    sfizz/Curve.cpp
    sfizz/Smoothers.cpp
    sfizz/Wavetables.cpp
    sfizz/Tuning.cpp
    sfizz/RegionSet.cpp
    sfizz/PolyphonyGroup.cpp
    sfizz/VoiceManager.cpp
    sfizz/VoiceStealing.cpp
    sfizz/RTSemaphore.cpp
    sfizz/Panning.cpp
    sfizz/Effects.cpp
    sfizz/LFO.cpp
    sfizz/LFODescription.cpp
    sfizz/PowerFollower.cpp
    sfizz/FlexEGDescription.cpp
    sfizz/FlexEnvelope.cpp
    sfizz/BeatClock.cpp
    sfizz/Metronome.cpp
    sfizz/SynthMessaging.cpp
    sfizz/WindowedSinc.cpp
    sfizz/Interpolators.cpp
    sfizz/modulations/ModId.cpp
    sfizz/modulations/ModKey.cpp
    sfizz/modulations/ModKeyHash.cpp
    sfizz/modulations/ModMatrix.cpp
    sfizz/modulations/sources/Controller.cpp
    sfizz/modulations/sources/FlexEnvelope.cpp
    sfizz/modulations/sources/ADSREnvelope.cpp
    sfizz/modulations/sources/LFO.cpp
    sfizz/utility/SpinMutex.cpp
    sfizz/effects/Nothing.cpp
    sfizz/effects/Filter.cpp
    sfizz/effects/Eq.cpp
    sfizz/effects/Apan.cpp
    sfizz/effects/Lofi.cpp
    sfizz/effects/Limiter.cpp
    sfizz/effects/Compressor.cpp
    sfizz/effects/Gate.cpp
    sfizz/effects/Disto.cpp
    sfizz/effects/Strings.cpp
    sfizz/effects/Fverb.cpp
    sfizz/effects/Rectify.cpp
    sfizz/effects/Gain.cpp
    sfizz/effects/Width.cpp
    sfizz/effects/impl/ResonantString.cpp
    sfizz/effects/impl/ResonantStringSSE.cpp
    sfizz/effects/impl/ResonantStringAVX.cpp
    sfizz/effects/impl/ResonantArray.cpp
    sfizz/effects/impl/ResonantArraySSE.cpp
    sfizz/effects/impl/ResonantArrayAVX.cpp)

include(SfizzSIMDSourceFiles)
sfizz_add_simd_sources(SFIZZ_SOURCES ".")

# Parser core library
set(SFIZZ_PARSER_HEADERS
    sfizz/Defaults.h
    sfizz/LeakDetector.h
    sfizz/Range.h
    sfizz/Opcode.h
    sfizz/Macros.h
    sfizz/Parser.h
    sfizz/parser/Parser.h
    sfizz/parser/ParserPrivate.h
    sfizz/parser/ParserPrivate.hpp
    sfizz/SfzHelpers.h
    sfizz/StringViewHelpers.h)

set(SFIZZ_PARSER_SOURCES
    sfizz/Parser.cpp
    sfizz/Opcode.cpp
    sfizz/OpcodeCleanup.cpp
    sfizz/SfzHelpers.cpp
    sfizz/parser/Parser.cpp
    sfizz/parser/ParserPrivate.cpp)

set(SFIZZ_PARSER_OTHER sfizz/OpcodeCleanup.re)
source_group("Other Files" FILES ${SFIZZ_PARSER_OTHER})

# Sfizz parser library
add_library(sfizz_parser STATIC)
add_library(sfizz::parser ALIAS sfizz_parser)
target_sources(sfizz_parser PRIVATE
    ${SFIZZ_PARSER_HEADERS} ${SFIZZ_PARSER_SOURCES} ${SFIZZ_PARSER_OTHER})
target_include_directories(sfizz_parser PUBLIC sfizz)
target_link_libraries(sfizz_parser
    PUBLIC sfizz::filesystem absl::strings
    PRIVATE absl::flat_hash_map)

# OSC messaging library
set(SFIZZ_MESSAGING_HEADERS
    sfizz/Messaging.h
    sfizz/Messaging.hpp
    sfizz_message.h)

set(SFIZZ_MESSAGING_SOURCES
    sfizz/Messaging.cpp)

add_library(sfizz_messaging STATIC)
add_library(sfizz::messaging ALIAS sfizz_messaging)
target_sources(sfizz_messaging PRIVATE
    ${SFIZZ_MESSAGING_HEADERS} ${SFIZZ_MESSAGING_SOURCES})
target_include_directories(sfizz_messaging PUBLIC ".")
target_link_libraries(sfizz_messaging PUBLIC absl::strings)

# Sfizz internals (use this for testing)
add_library(sfizz_internal STATIC)
add_library(sfizz::internal ALIAS sfizz_internal)
target_sources(sfizz_internal PRIVATE ${SFIZZ_HEADERS} ${SFIZZ_SOURCES} ${FAUST_FILES})
target_include_directories(sfizz_internal PUBLIC "." "sfizz")
target_link_libraries(sfizz_internal
    PUBLIC absl::strings absl::span sfizz::filesystem sfizz::atomic_queue
    PRIVATE sfizz::parser sfizz::messaging absl::flat_hash_map Threads::Threads st_audiofile sfizz::pugixml sfizz::spline sfizz::tunings sfizz::hiir sfizz::kissfft sfizz::cephes sfizz::cpuid sfizz::threadpool sfizz::jsl sfizz::atomic)
if(SFIZZ_USE_SNDFILE)
    target_compile_definitions(sfizz_internal PUBLIC "SFIZZ_USE_SNDFILE=1")
    target_link_libraries(sfizz_internal PUBLIC st_audiofile)
endif()
if(WIN32)
    target_compile_definitions(sfizz_internal PRIVATE _USE_MATH_DEFINES)
endif()
if(SFIZZ_RELEASE_ASSERTS)
    target_compile_definitions(sfizz_internal PUBLIC "SFIZZ_ENABLE_RELEASE_ASSERT=1")
endif()
sfizz_enable_fast_math(sfizz_internal)

# Sfizz static library
add_library(sfizz_static STATIC sfizz/sfizz_wrapper.cpp sfizz/sfizz.cpp)
add_library(sfizz::static ALIAS sfizz_static)
target_include_directories(sfizz_static PUBLIC .)
target_link_libraries(sfizz_static PRIVATE sfizz::internal)
set_target_properties(sfizz_static PROPERTIES OUTPUT_NAME "sfizz" PUBLIC_HEADER "sfizz.h;sfizz.hpp;sfizz_message.h")

# Shared library and installation target
if(SFIZZ_SHARED)
    add_library(sfizz_shared SHARED sfizz/sfizz_wrapper.cpp sfizz/sfizz.cpp)
    add_library(sfizz::shared ALIAS sfizz_shared)
    target_include_directories(sfizz_shared PUBLIC .)
    target_link_libraries(sfizz_shared PRIVATE sfizz::internal)
    target_compile_definitions(sfizz_shared PRIVATE SFIZZ_EXPORT_SYMBOLS)
    set_target_properties(sfizz_shared PROPERTIES SOVERSION "${PROJECT_VERSION_MAJOR}" OUTPUT_NAME "sfizz" PUBLIC_HEADER "sfizz.h;sfizz.hpp;sfizz_message.h")
    sfizz_enable_lto_if_needed(sfizz_shared)

    if(NOT MSVC)
        install(TARGETS sfizz_shared
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT "runtime"
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT "runtime"
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT "development"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT "development")
        configure_file(${PROJECT_SOURCE_DIR}/scripts/sfizz.pc.in sfizz.pc @ONLY)
        install(FILES ${CMAKE_BINARY_DIR}/src/sfizz.pc
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
            COMPONENT "development")
    endif()
endif()

# Generic library alias
add_library(sfizz::sfizz ALIAS sfizz_static)

# Windows installer
if(WIN32)
    include(VSTConfig)
    configure_file(${PROJECT_SOURCE_DIR}/scripts/innosetup.iss.in ${PROJECT_BINARY_DIR}/innosetup.iss @ONLY)
endif()

# Doxygen
configure_file(${PROJECT_SOURCE_DIR}/scripts/doxygen/Doxyfile.in ${PROJECT_SOURCE_DIR}/Doxyfile @ONLY)
