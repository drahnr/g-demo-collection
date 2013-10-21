CC ?= gcc
PKGS := glib-2.0 gio-2.0 libsoup-2.4
CFLAGS += -ggdb -Wall $(shell pkg-config --static --cflags $(PKGS))
LDFLAGS += -ggdb
LIBS += $(shell pkg-config --static --libs $(PKGS))

BUILDDIR := build
SOURCES := $(shell find src/ -iname "*.c" -type f)
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.c=%.o))
BINARY := soup-auth-server.bin

$(BINARY): $(OBJECTS)
	@echo $(CFLAGS)
	@echo $(LDFLAGS)
	@echo $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(BINARY) $(LIBS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Isrc -I$(dir $<) -c $< -o $@

clean:
	rm -rvf build
