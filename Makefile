CXX	:= g++
CXXFLAGS := $(CXXFLAGS) -O3 -Wall -std=c++11

SOURCEDIR := ./src
OBJECTDIR := ./obj
BINDIR := ./bin
TARGET := a1106307_lab4

SOURCE_FILES := $(wildcard $(SOURCEDIR)/*.cpp) $(wildcard $(SOURCEDIR)/lib/tinyxml2/*.cpp)
OBJECT_FILES := $(subst $(SOURCEDIR)/, $(OBJECTDIR)/, $(SOURCE_FILES:.cpp=.o))
#~ DEPS := $(OBJECT_FILES)
DEPS := $(OBJECT_FILES:.o=.d)

.PHONY: all run clean

all: $(BINDIR)/$(TARGET)
-include $(DEPS)

$(OBJECTDIR)/%.d: $(SOURCEDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) -MM -MT "$@ $(patsubst) %.d,%.o,$@)" -MF $@ $<

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	@echo compiling
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR)/$(TARGET): $(SOURCE_FILES)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

run:
	bin/$(TARGET) xml/example1.xml

clean:
	-@rm -r obj
	-@rm -r bin
