using System.Threading;
using System;
class Program
{
    static void Plus()
    {
        int check = 0;
        while (true)
        {
            if (check >= 50)
            {
                Console.WriteLine();
                check = 0;
            }
            Console.Write("+");
            Thread.Sleep(10);
            check++;
        }
    }
    static void Minus()
    {
        int check = 0;
        while (true)
        {
            if (check >= 50)
            {
                Console.WriteLine();
                check = 0;
            }
            Console.Write("-");
            Thread.Sleep(10);
        }
    }
    static void Main()
    {
        Thread t1 = new Thread(Plus);
        Thread t2 = new Thread(Minus);
        t1.Start();
        t2.Start();
    }
}

