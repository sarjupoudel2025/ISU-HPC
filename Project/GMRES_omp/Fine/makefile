# Compiler
CC = gcc

# Base flags
CFLAGS = -Wall -Wextra -O2 -MMD -MP

# OpenMP flags
OMPFLAGS = -fopenmp

# Targets
TARGET_OMP = solve_omp

# Output folders/files
OMP_OUTDIR    = omp_vtk
OMP_POSTPRO    = postpro_omp.out

# Common source files (NO main here!)
COMMON_SRCS = mesh.c field.c io.c vtk.c solver.c operator.c linalg.c postpro.c utils.c

# Main file
MAIN_OMP    = main_omp.c

# Object files
COMMON_OBJS = $(COMMON_SRCS:.c=.o)
OBJS_OMP    = $(MAIN_OMP:.c=.o) $(COMMON_OBJS)

# Dependency files
DEPS = $(COMMON_OBJS:.o=.d) $(MAIN_OMP:.c=.d)

# Default target
all: $(TARGET_OMP)

# -------------------------------
# OPENMP BUILD
# -------------------------------
$(TARGET_OMP): $(OBJS_OMP)
	$(CC) $(CFLAGS) $(OMPFLAGS) -o $@ $(OBJS_OMP) -lm

# -------------------------------
# Compile rules
# -------------------------------
%.o: %.c
	$(CC) $(CFLAGS) $(OMPFLAGS) -c $< -o $@

# -------------------------------
# Dependencies
# -------------------------------
-include $(DEPS)

# -------------------------------
# Clean
# -------------------------------

clean:
	rm -f *.o *.d $(TARGET_OMP)
	rm -f *.vtk postpro.out $(OMP_POSTPRO)
	rm -rf $(OMP_OUTDIR)

clean_omp:
	rm -f main_omp.o main_omp.d $(TARGET_OMP)

clean_omp_run:
	@if [ -n "$(THREADS)" ]; then \
		rm -f $(OMP_OUTDIR)/t$(THREADS)/postpro_omp.out $(OMP_OUTDIR)/t$(THREADS)/run.log; \
		rm -rf $(OMP_OUTDIR)/t$(THREADS); \
	else \
		echo "Usage: make clean_omp_run THREADS=<n>"; \
	fi

# -------------------------------
# Rebuild
# -------------------------------
rebuild: clean all