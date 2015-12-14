CXX	:= g++
CXXFLAGS := $(CXXFLAGS) -O3 -Wall -std=c++11

SOURCEDIR := ./src
OBJECTDIR := ./obj
BINDIR := ./bin
TARGET := a1106307_lab4

SOURCE_FILES := $(wildcard $(SOURCEDIR)/*.cpp) \
		$(wildcard $(SOURCEDIR)/lib/tinyxml2/*.cpp) \
		$(wildcard $(SOURCEDIR)/io/*.cpp) \
		$(wildcard $(SOURCEDIR)/graphics/*.cpp)
OBJECT_FILES := $(subst $(SOURCEDIR)/, $(OBJECTDIR)/, $(SOURCE_FILES:.cpp=.o))
DEPS := $(OBJECT_FILES:.o=.d)

.PHONY: all run clean

all: $(BINDIR)/$(TARGET)
$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
-include $(DEPS)

$(OBJECTDIR)/%.d: $(SOURCEDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) -MM -MT "$@ $(patsubst) %.d,%.o,$@)" -MF $@ $<

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	@echo compiling $< ...
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $< -o $@

$(BINDIR)/$(TARGET): $(OBJECT_FILES)
	mkdir -p $(BINDIR)
	$(CXX) -o $@ $^

run:
	bin/$(TARGET) xml/example1.xml

clean:
	-@rm -f -r $(OBJECTDIR)
	-@rm -f -r $(BINDIR)
	-@rm -f $(SOURCEDIR)/lib/tinyxml2/*.o
	-@rm -f $(SOURCEDIR)/*.o $(SOURCEDIR)/*.gch
	-@rm -f $(SOURCEDIR)/io/*.o $(SOURCEDIR)/io/*.gch
	-@rm -f $(SOURCEDIR)/graphics/*.o $(SOURCEDIR)/graphics/*.gch
	-@rm -f *.ppm
