CC ?= gcc
PKGS := gtk+-3.0 goocanvas-2.0
CFLAGS += -ggdb -Wall $(shell pkg-config --cflags $(PKGS))
LDFLAGS += -ggdb
LIBS += $(shell pkg-config --libs $(PKGS))

BUILDDIR := build
SOURCES := $(shell find ./src -iname "*.c" -type f)
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.c=%.o))
BINARY := simple-canvas-goo.bin

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(BINARY) $(LIBS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Isrc -I$(dir $<) -c $< -o $@

clean:
	rm -rvf build
