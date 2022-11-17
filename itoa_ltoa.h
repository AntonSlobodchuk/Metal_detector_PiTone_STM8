void reverse(char s[]) // reverse:  �������������� ������ s �� �����
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 };

void itoa(int n, char s[]) // itoa:  ������������ n � ������� � s
{
     int i, sign;

     if ((sign = n) < 0)  // ���������� ����
         n = -n;          // ������ n ������������� ������
     i = 0;
     do {       // ���������� ����� � �������� �������
         s[i++] = n % 10 + '0';   // ����� ��������� �����
     } while ((n /= 10) > 0);     // �������
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}

void ltoa(long n, char s[]) // ltoa:  ������������ n � ������� � s
{
     int i;
     long sign;

     if ((sign = n) < 0)  // ���������� ����
         n = -n;          // ������ n ������������� ������
     i = 0;
     do {       // ���������� ����� � �������� �������
         s[i++] = n % 10 + '0';   // ����� ��������� �����
     } while ((n /= 10) > 0);     // �������
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
};

float ascii_to_float(char *ascii){
	unsigned char i=0, n=0, znakov_do_tochki=0, znakov_posle_tochki=0;
	float atof_temp, atof_ret;

	while(1^((ascii[znakov_do_tochki]==0)|(ascii[znakov_do_tochki]==0x20)|(ascii[znakov_do_tochki]==0x2e))){ znakov_do_tochki++; }; //���� � ������ "NULL" ��� "������� �������"
	if(znakov_do_tochki>0){ while(1^((ascii[(znakov_do_tochki+1)+znakov_posle_tochki]==0)|(ascii[(znakov_do_tochki+1)+znakov_posle_tochki]==0x20))){ znakov_posle_tochki++; }; }; //���� � ������ "NULL" ��� "������� �������"

	if(znakov_do_tochki>0){
		for(i=0; i<znakov_do_tochki; i++){//���� � ������ "NULL" ��� "�����", ����� ������� ����������������

			atof_temp=(ascii[i]-0x30);
			for(n=0; n<((znakov_do_tochki-1)-i); n++){atof_temp=atof_temp*10;};
			atof_ret=atof_ret+atof_temp;
										 };
	                      };

	if(znakov_posle_tochki>0){
			for(i=0; i<znakov_posle_tochki; i++){//���� � ������ "NULL" ��� "�����", ����� ������� ����������������

				atof_temp=(ascii[(znakov_do_tochki+1)-i]-0x30);
				for(n=0; n<i+1; n++){atof_temp=atof_temp * 0.1;};

				atof_ret=atof_ret+atof_temp;
											 };
		                      };
	return atof_ret;
};
