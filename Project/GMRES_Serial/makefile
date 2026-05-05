# Compiler
CC = gcc

# Base flags
CFLAGS = -Wall -Wextra -O2 -MMD -MP

# OpenMP flags
OMPFLAGS = -fopenmp

# Compiler
CC = gcc

# Base flags
CFLAGS = -Wall -Wextra -O2 -MMD -MP

# Targets
TARGET_SERIAL = solve_serial

# Output folders/files
SERIAL_OUTDIR = serial_vtk
SERIAL_POSTPRO = postpro_serial.out

# Common source files (NO main here!)
COMMON_SRCS = mesh.c field.c io.c vtk.c solver.c operator.c linalg.c postpro.c utils.c

# Main file
MAIN_SERIAL = main_serial.c

# Object files
COMMON_OBJS = $(COMMON_SRCS:.c=.o)
OBJS_SERIAL = $(MAIN_SERIAL:.c=.o) $(COMMON_OBJS)

# Dependency files
DEPS = $(COMMON_OBJS:.o=.d) $(MAIN_SERIAL:.c:.d)

# Default target
all: $(TARGET_SERIAL)

# -------------------------------
# SERIAL BUILD
# -------------------------------
$(TARGET_SERIAL): $(OBJS_SERIAL)
	$(CC) $(CFLAGS) -o $@ $(OBJS_SERIAL) -lm

# -------------------------------
# Compile rules
# -------------------------------
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# -------------------------------
# Dependencies
# -------------------------------
-include $(DEPS)

# -------------------------------
# Clean
# -------------------------------
clean:
	rm -f *.o *.d $(TARGET_SERIAL)
	rm -f *.vtk postpro.out $(SERIAL_POSTPRO)
	rm -rf $(SERIAL_OUTDIR)

clean_serial:
	rm -f main_serial.o main_serial.d $(TARGET_SERIAL)
	rm -f $(SERIAL_POSTPRO)
	rm -rf $(SERIAL_OUTDIR)

# -------------------------------
# Rebuild
# -------------------------------
rebuild: clean all