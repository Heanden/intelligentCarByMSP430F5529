void Gesrure_test(void)
{
    u8 i;
    u8 status;
    u8 key;
    u8 data[2] = {0x00};
    u16 gesture_data;
    u8 ledflash = 0;

    paj7620u2_selectBank(BANK0); //����BANK0
    for (i = 0; i < GESTURE_SIZE; i++)
    {
        GS_Write_Byte(gesture_arry[i][0], gesture_arry[i][1]); //����ʶ��ģʽ��ʼ��
    }
    paj7620u2_selectBank(BANK0); //�л���BANK0
    i = 0;
    POINT_COLOR = BLUE; //��������Ϊ��ɫ
    LCD_Fill(30, 170, 300, 300, WHITE);
    LCD_ShowString(30, 180, 200, 16, 16, "KEY_UP: Exit the test");
    LCD_ShowString(30, 210, 200, 16, 16, "Gesrure test");
    POINT_COLOR = RED; //��������Ϊ��ɫ
    while (1)
    {
        key = KEY_Scan(0);
        if (key == WKUP_PRES)
        {
            GS_Write_Byte(PAJ_SET_INT_FLAG1, 0X00); //�ر�����ʶ���ж����
            GS_Write_Byte(PAJ_SET_INT_FLAG2, 0X00);
            break;
        }
        status = GS_Read_nByte(PAJ_GET_INT_FLAG1, 2, &data[0]); //��ȡ����״̬
        if (!status)
        {
            gesture_data = (u16)data[1] << 8 | data[0];
            if (gesture_data)
            {
                switch (gesture_data)
                {
                case GES_UP:
                    LCD_ShowString(110, 250, 200, 16, 24, "UP          ");
                    printf("Up\r\n");
                    ledflash = 1;
                    break; //����
                case GES_DOWM:
                    LCD_ShowString(100, 250, 200, 16, 24, "Dowm        ");
                    printf("Dowm\r\n");
                    ledflash = 1;
                    break; //����
                case GES_LEFT:
                    LCD_ShowString(100, 250, 200, 16, 24, "Left        ");
                    printf("Left\r\n");
                    ledflash = 1;
                    break; //����
                case GES_RIGHT:
                    LCD_ShowString(100, 250, 200, 16, 24, "Right       ");
                    printf("Right\r\n");
                    ledflash = 1;
                    break; //����
                case GES_FORWARD:
                    LCD_ShowString(80, 250, 200, 16, 24, "Forward     ");
                    printf("Forward\r\n");
                    ledflash = 1;
                    break; //��ǰ
                case GES_BACKWARD:
                    LCD_ShowString(80, 250, 200, 16, 24, "Backward    ");
                    printf("Backward\r\n");
                    ledflash = 1;
                    break; //���
                case GES_CLOCKWISE:
                    LCD_ShowString(70, 250, 200, 16, 24, "Clockwise   ");
                    printf("Clockwise\r\n");
                    ledflash = 1;
                    break; //˳ʱ��
                case GES_COUNT_CLOCKWISE:
                    LCD_ShowString(50, 250, 200, 16, 24, "AntiClockwise");
                    printf("AntiClockwise\r\n");
                    ledflash = 1;
                    break; //��ʱ��
                case GES_WAVE:
                    LCD_ShowString(100, 250, 200, 16, 24, "Wave         ");
                    printf("Wave\r\n");
                    ledflash = 1;
                    break; //�Ӷ�
                default:
                    ledflash = 0;
                    break;
                }
                if (ledflash) //DS1��˸
                {
                    LED1 = 0;
                    delay_ms(80);
                    LED1 = 1;
                    delay_ms(80);
                    LED1 = 0;
                    delay_ms(80);
                    LED1 = 1;
                    delay_ms(80);
                    delay_ms(300);
                    LCD_ShowString(40, 250, 200, 16, 24, "                        ");
                    ledflash = 0;
                }
            }
        }
        delay_ms(50);
        i++;
        if (i == 5)
        {
            LED0 = !LED0; //��ʾϵͳ��������
            i = 0;
        }
    }
}
