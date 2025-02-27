if(NOT TARGET ${MQT_CORE_TARGET_NAME}-dd)
  file(GLOB_RECURSE DD_HEADERS ${MQT_CORE_INCLUDE_BUILD_DIR}/dd/*.hpp)

  # add DD Package library
  add_library(
    ${MQT_CORE_TARGET_NAME}-dd
    ${DD_HEADERS}
    Benchmark.cpp
    CachedEdge.cpp
    Complex.cpp
    ComplexNumbers.cpp
    ComplexValue.cpp
    Edge.cpp
    FunctionalityConstruction.cpp
    MemoryManager.cpp
    Node.cpp
    NoiseFunctionality.cpp
    Operations.cpp
    RealNumber.cpp
    RealNumberUniqueTable.cpp
    Simulation.cpp
    statistics/MemoryManagerStatistics.cpp
    statistics/Statistics.cpp
    statistics/TableStatistics.cpp
    statistics/UniqueTableStatistics.cpp)
  target_link_libraries(${MQT_CORE_TARGET_NAME}-dd PUBLIC MQT::Core)
  add_library(MQT::CoreDD ALIAS ${MQT_CORE_TARGET_NAME}-dd)
  set_target_properties(mqt-core-dd PROPERTIES EXPORT_NAME CoreDD)
  set(MQT_CORE_TARGETS
      ${MQT_CORE_TARGETS} ${MQT_CORE_TARGET_NAME}-dd
      PARENT_SCOPE)
endif()
