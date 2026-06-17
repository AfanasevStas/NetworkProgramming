using System.Threading;
using System;
class Program
{
    static Mutex mutex = new Mutex();
    static int check = 0;
    static void Line()
    {
        while (true)
        {
            if (check >= 50)
            {
                mutex.WaitOne();
                Console.WriteLine();
                mutex.ReleaseMutex();
                check = 0;
            }

        }
    }
        static void Plus()
    {
        while (true)
        {
                mutex.WaitOne();
            Console.Write("+");
            check++;
            Thread.Sleep(10);
                mutex.ReleaseMutex();
        }
    }
    static void Minus()
    {
        while (true)
        {
                mutex.WaitOne();
            Console.Write("-");
            check++;
            Thread.Sleep(10);
                mutex.ReleaseMutex();
        }
    }
    static void Main()
    {
        Thread t1 = new Thread(Plus);
        Thread t2 = new Thread(Minus);
        Thread t3 = new Thread(Line);
        t1.Start();
        t2.Start();
        t3.Start();
    }
}

