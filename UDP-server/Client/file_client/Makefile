
SOURCES=main.cpp iknlib.c
OBJECTS=build/x86-64/main.o build/x86-64/iknlib.o
DEPS=build/x86-64/main.d build/x86-64/iknlib.d
EXE=file_client
CXXFLAGS=-I.
ARCH?=arm
# Making for x86 -64 e.g. x86 -64 (the architecture employed)
# > make ARCH=x86 -64
ifeq (${ARCH},x86-64)
CXX=g++
BUILD_DIR=build/x86-64
endif
# Making for architecture
# > make ARCH=arm
ifeq (${ARCH},arm)
CXX=arm-rpizw-g++
BUILD_DIR=build/arm
endif
$(BUILD_DIR)/$(EXE): $(DEPS) $(OBJECTS) # << Check the $(DEPS) new dependency
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)
	echo $(BUILD_DIR)/$(EXE)
# %.cpp -> %.o needs to be added! Target is NOT just %.o...
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@
# Rule that describes how a .d (dependency) file is created from a .cpp file
# Similar to the assigment that you just completed %.cpp -> %.o
$(BUILD_DIR)/%.d: %.cpp
	$(CXX) -MT$@ -MM $(CXXFLAGS) $< > $@
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $< >> $@

$(BUILD_DIR)/%.d: %.c
	$(CXX) -MT$@ -MM $(CXXFLAGS) $< > $@
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $< >> $@
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif