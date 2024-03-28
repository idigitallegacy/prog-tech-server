cmake_minimum_required(VERSION 3.27)
project(DotaParserServer)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(DotaParserServer main.cpp
            Server/core/Server.cpp
            Server/core/Server.h
            Server/ServerPlugin.cpp
            Server/ServerPlugin.h
            Server/processor/PostRequestProcessor.cpp
            Server/processor/PostRequestProcessor.h
            Server/storage/TimelineJsonStorage.cpp
            Server/storage/TimelineJsonStorage.h
            Server/processor/GetRequestProcessor.cpp
            Server/processor/GetRequestProcessor.h
            Server/struct/ServerOptions.h
            Server/processor/ItemsProcessor.cpp
            Server/processor/ItemsProcessor.h
            cfg.h
            Server/include/BoostJsonInclude.h
            Server/include/StlInclude.h
            Server/include/BoostBeastInclude.h
            definitions.h
            Server/executor/SaveExecutor.cpp
            Server/executor/SaveExecutor.h
            Server/executor/LoadExecutor.cpp
            Server/executor/LoadExecutor.h
    )
    target_link_libraries(DotaParserServer ${Boost_LIBRARIES} )

    enable_testing()
    find_package(GTest CONFIG REQUIRED)

    add_executable(
            DotaParserServerTest

            Server/core/Server.cpp
            Server/core/Server.h
            Server/ServerPlugin.cpp
            Server/ServerPlugin.h
            Server/processor/PostRequestProcessor.cpp
            Server/processor/PostRequestProcessor.h
            Server/storage/TimelineJsonStorage.cpp
            Server/storage/TimelineJsonStorage.h
            Server/processor/GetRequestProcessor.cpp
            Server/processor/GetRequestProcessor.h
            Server/struct/ServerOptions.h
            Server/processor/ItemsProcessor.cpp
            Server/processor/ItemsProcessor.h
            cfg.h
            Server/include/BoostJsonInclude.h
            Server/include/StlInclude.h
            Server/include/BoostBeastInclude.h
            definitions.h
            Server/executor/SaveExecutor.cpp
            Server/executor/SaveExecutor.h
            Server/executor/LoadExecutor.cpp
            Server/executor/LoadExecutor.h
    )


    target_link_libraries(
            DotaParserServerTest
            GTest::gtest
            GTest::gtest_main
            GTest::gmock
            GTest::gmock_main
            ${Boost_LIBRARIES}
    )

    include(GoogleTest)
    gtest_discover_tests(DotaParserServerTest)
endif()