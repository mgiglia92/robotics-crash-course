Add the following line to the root CMakeLists.txt, before including the Pico
SDK:
`add_definitions(-DPICO_MAX_SHARED_IRQ_HANDLERS=16u)`
