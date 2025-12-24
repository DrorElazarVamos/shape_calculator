CC = gcc
CFLAGS = -Wall -Wextra -g 
TARGET = matrice_app
OBJS = test_main.o matrice.o

all: $(TARGET)

# FIX: Split into two lines.
# The second line MUST start with a TAB key, not spaces.
$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) 

# FIX: Added ':' between %.o and %.c
# The second line MUST start with a TAB key.
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS) $(TARGET)