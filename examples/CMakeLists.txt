add_executable(ExampleApp ExampleApp.cpp)
target_link_libraries(ExampleApp PRIVATE InariKonKon::InariKonKon)

add_executable(TimeExample TimeExamples.cpp)
target_link_libraries(TimeExample PRIVATE InariKonKon::InariKonKon)

add_executable(MathExample MathExamples.cpp)
target_link_libraries(MathExample PRIVATE InariKonKon::InariKonKon)

add_executable(UtilityExample UtilityExamples.cpp)
target_link_libraries(UtilityExample PRIVATE InariKonKon::InariKonKon)

add_executable(InputExample InputExamples.cpp)
target_link_libraries(InputExample PRIVATE InariKonKon::InariKonKon)
