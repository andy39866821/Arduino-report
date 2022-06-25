const int ROW_PIN[8] = {3, 6, 13, 7, 11, A2, A1, 8};
const int COL_PIN[8] = {12, 5, 4, 9, 2, 10, A0, A3};
bool led_map[8][8];
void setup()
{
}
void loop()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (led_map[i][j] == 1)
            {
                digitalWrite(ROW_PIN[i], HIGH);
                digitalWrite(COL_PIN[j], LOW);
                _delay_us(100);
                digitalWrite(ROW_PIN[i], LOW);
                digitalWrite(COL_PIN[j], HIGH);
            }
        }
    }
}