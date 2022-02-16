include Makefile.inc

LIBS = app stream mcu utils event store ocpp cjson gsm stream mcu utils event store ocpp cjson gsm stream mcu utils event store ocpp cjson gsm app

CXXFLAGS = $(PRJCXXFLAGS) -I.

SRCDIR = .
DEPDIR = .dep
OBJDIR = obj

SOURCE = $(wildcard $(SRCDIR)/*.cc)
OBJS = $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(SOURCE:.cc=.o))
DEPS = $(patsubst $(OBJDIR)%, $(DEPDIR)%, $(OBJS:.o=.d))
LIBA = $(patsubst %, $(OBJDIR)/lib%.a, $(LIBS))
LIBL = -L$(OBJDIR) $(patsubst %, -l%, $(LIBS))

TARGET 	= $(notdir $(CURDIR))
ELF 	= $(notdir $(CURDIR)).elf

all: $(TARGET)

clean:
	-@for lib in $(LIBS); do (cd $$lib; $(MAKE) clean); done
	-@rm -rf obj
	-@rm -rf .dep
	-@rm $(TARGET)

$(DEPDIR)/%.d: %.cc
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM $< > $@
	@sed -i -e "s,$*.o:,$(OBJDIR)/$*.o $@: ,g" $@

$(OBJDIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	@echo [C++] $(notdir $<)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.a: force_look
	@cd $(patsubst lib%, %, $(basename $(notdir $@)) ; $(MAKE) $(MFLAGS))

$(OBJDIR)/$(ELF): $(LIBA) $(OBJS)
	@echo [LNK] $(notdir $@)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(LIBL) -o $@
	@echo
	@$(SIZE) $(SIZEFLAGS) $@

%: $(OBJDIR)/%.elf
	@echo [HEX] $@
	@$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

force_look:
	@true

-include $(DEPS)
