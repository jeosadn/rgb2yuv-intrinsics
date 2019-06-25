PREFIX ?= /usr
BINDIR ?= bin
INSTALL ?= install
CFLAGS += -mfpu=neon
LDLAGS += 

TARGET = rgb2yuv-intrinsics

all: $(TARGET)

%: %.c
	arm-linux-gnueabihf-g++ $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	$(RM) $(TARGET)

install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/$(BINDIR)
	$(INSTALL) $(TARGET) $(DESTDIR)$(PREFIX)/$(BINDIR)/$(TARGET)
