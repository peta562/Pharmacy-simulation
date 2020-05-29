#include "Pharmacy.h"

#define N 10000 // Время моделирования

void TestModule()
{
    ModelParamaters mp;
    
    mp.num_inbank.open("NumInBank.txt");
    mp.time_outbank.open("TimeOutBank.txt");
    mp.sojourn.open("SOJORN.txt");
    mp.dump.open("DUMP.txt");
    mp.total = 0;

    Pharmacy ph(&mp);
   
    while (mp.total < N)
    {
        ph.Run();
        mp.total++;
    }

    cout << "Всего поступлений в систему " << mp.entered << endl;
    cout << "Завершили обслуживание " << mp.completed << endl;
    cout << "Доля отвергнутых заявок " << (float)mp.reject / mp.entered << endl;
    cout << "Количество переходов " << mp.transition << endl;
    cout << "Средняя длина первой очереди " << mp.que1Ave << endl;
    cout << "Средняя длина второй очереди " << mp.que2Ave << endl;
    cout << "Средняя длина заявок в системе " << mp.numAve << endl;
    cout << "Средняя время пребывания в системе " << mp.sojAve << endl;
    cout << "Средний интервал между выходами заявок " << mp.outAve << endl;
    cout << "Средняя число занятых кассиров " << mp.roAve << endl;

    mp.num_inbank.close();
    mp.time_outbank.close();
    mp.sojourn.close();
    mp.dump.close();
}

int main()
{
    srand((unsigned)time(0));
    setlocale(LC_ALL, "Russian");
    TestModule();

    return 0;
}

