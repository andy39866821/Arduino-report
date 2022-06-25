
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define ROLL_X_PIN A0
#define ROLL_Y_PIN A1
int move_direction = LEFT;

void setup()
{
}

void loop()
{
    get_roll_direction();
}
void get_roll_direction()
{

    int x = analogRead(ROLL_X_PIN);
    int y = analogRead(ROLL_Y_PIN);

    if (x > 900)
    {
        move_direction = DOWN;
    }
    else if (x < 100)
    {
        move_direction = UP;
    }
    else if (y > 900)
    {
        move_direction = LEFT;
    }
    else if (y < 100)
    {
        move_direction = RIGHT;
    }
}