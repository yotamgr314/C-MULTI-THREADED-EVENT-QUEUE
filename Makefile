# שם הקובץ שנוצר אחרי הקומפילציה
TARGET = async_printer

# קומפיילר והדגלים
CC = gcc
CFLAGS = -Wall -pthread -g

# קבצי המקור והכותרות
SRC = main.c shared_event_queue.c event_node.c
OBJ = $(SRC:.c=.o)

# ברירת מחדל: קומפילציה והרצה
all: $(TARGET) run

# קישור כל קובצי האובייקט ליצירת קובץ הרצה
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# קומפילציה של כל קובצי המקור
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# הרצת התוכנית
run: $(TARGET)
	./$(TARGET)

# ניקוי הקבצים שנוצרו
clean:
	rm -f $(OBJ) $(TARGET)
