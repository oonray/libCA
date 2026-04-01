#
# LibCA
#
name=libCA

#SOURCES
SRC_D=./src
DATA_D=$(SRC_D)/data
STATIC_D=$(BUILD_D)/lib

#BUILD
BUILD_D:=./build
OBJ_BD:=$(BUILD_D)/obj
TEST_BD:=$(BUILD_D)/tests
LIB_BD:=$(BUILD_D)/lib
DIRS:=$(OBJ_BD) $(TEST_BD) $(LIB_BD) $(LIB_BD)

#COMPILE
CC:=gcc
CMD_C:=./compile_commands.json
BEAR_C:=bear --append --output $(CMD_C) --

VPATH:=$(VPATH):src:lib:$(wildcard lib/**):tests:build:$(OBJ_BD):$(LIB_BD)
FLAGS:=$(FLAGS) $(patsubst %,-I%,$(subst :, ,$(VPATH))) -Wall

#INSTALL
IND=/usr/
H_IND=$(IND)/include/
LIB_IND=$(IND)/lib32/

.PHONY: all
all: debug

.PHONY: test
test: FLAGS:=$(FLAGS) -D TESTS -O0 -g
test: CC:=$(BEAR_C) $(CC)
test: dirs tests

.PHONY: debug
debug: FLAGS:=$(FLAGS) -D DEBUG -O0 -g
debug: CC:=$(BEAR_C) $(CC)
debug: dirs tests

.PHONY: release
release: BUILD_D:=./build/release
release: OBJ_BD:=$(BUILD_D)/obj
release: LIB_BD:=$(BUILD_D)/lib
release: DIRS:=$(OBJ_BD) $(LIB_BD) $(LIB_BD)
release: FLAGS:=$(FLAGS) -O3 -D RELEASE
release: dirs

install: clean release $(H_FILES) $(LIB_FILES)
	cp -t $(H_FILES) $(H_IND)
	cp -r $(LIB_BD) $(LIB_IND)

uninstall:
	rm $(patsubst %,$(H_IND)/$(notdir $(H_FILES)))
	rm $(patsubst %,$(LIB_IND)/% $(notdir $(LIB_BD)))

clean:
	rm -rf $(BUILD_D)

dirs:
	mkdir -p $(DIRS)

#RULES
%.o: %.c %.h
	$(CC) $(FLAGS) -c -o $(OBJ_BD)/$@ $<

%.o: %.c %.h
	$(CC) $(FLAGS) -c -o $(OBJ_BD)/$@ $<

lib%.a: %.o
	ar rcs $(LIB_BD)/$@ $(OBJ_BD)/$<

#LIBS
.PHONY: bstrlib
bstrlib: LIB_FILES:=$(LIB_FILES) libbstrlib.a
bstrlib: libbstrlib.a

.PHONY: munit
munit: LIB_FILES:=$(LIB_FILES) libmunit.a
munit: libmunit.a

.PHONY: mongoose
mongoose: LIB_FILES:=$(LIB_FILES) libmongoose.a
mongoose: libmongoose.a

.PHONY: tests
tests: TESTS:=src/ca_vector.c
tests: tests/main.c munit bstrlib ca_vector.o src/ca_vector.h
	$(CC) $(FLAGS) -o $(TEST_BD)/$@ $< $(TESTS) -L $(LIB_BD) -lmunit

#TESTS
