CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
INCLUDES = -Iinclude
LDFLAGS = -lpthread

SRC_DIR = src
BUILD_DIR = build
TARGET = simple-server

SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@echo "Linking $@..."
	@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete!"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory"

run: all
	@./$(BUILD_DIR)/$(TARGET) 38080 38081

debug:
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"

PACKAGE_NAME = simple-server_1.0.0_amd64
PACKAGE_DIR = package

deb: all
	@echo "Creating .deb package..."
	@rm -rf /tmp/$(PACKAGE_DIR)
	@mkdir -p /tmp/$(PACKAGE_DIR)/DEBIAN
	@mkdir -p /tmp/$(PACKAGE_DIR)/usr/local/bin
	@mkdir -p /tmp/$(PACKAGE_DIR)/etc/systemd/system
	@cp $(BUILD_DIR)/$(TARGET) /tmp/$(PACKAGE_DIR)/usr/local/bin/
	@cp simple-server.service /tmp/$(PACKAGE_DIR)/etc/systemd/system/
	@echo "Package: simple-server" > /tmp/$(PACKAGE_DIR)/DEBIAN/control
	@echo "Version: 1.0.0" >> /tmp/$(PACKAGE_DIR)/DEBIAN/control
	@echo "Section: net" >> /tmp/$(PACKAGE_DIR)/DEBIAN/control
	@echo "Priority: optional" >> /tmp/$(PACKAGE_DIR)/DEBIAN/control
	@echo "Architecture: amd64" >> /tmp/$(PACKAGE_DIR)/DEBIAN/control
	@echo "Maintainer: NF <nickfillick@yandex.ru>" >> /tmp/$(PACKAGE_DIR)/DEBIAN/control
	@echo "Description: Epoll-based TCP/UDP server" >> /tmp/$(PACKAGE_DIR)/DEBIAN/control
	@echo " Asynchronous server with epoll multiplexing" >> /tmp/$(PACKAGE_DIR)/DEBIAN/control
	@chmod 755 /tmp/$(PACKAGE_DIR)/DEBIAN
	@chmod 644 /tmp/$(PACKAGE_DIR)/DEBIAN/control
	@dpkg-deb --build /tmp/$(PACKAGE_DIR) $(PACKAGE_NAME).deb
	@rm -rf /tmp/$(PACKAGE_DIR)
	@echo "Package created: $(PACKAGE_NAME).deb"

.PHONY: all clean run debug deb