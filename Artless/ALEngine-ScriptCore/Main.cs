using System;

namespace ALEngine
{
    public class Main
    {
        public float FloatVar { get; set; }

        public Main()
        {
            Console.WriteLine("Main constructed! ADSA");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello world from C#! HMM");
        }

        public void PrintCustomMessage(string msg)
        {
            Console.WriteLine($"C# says: {msg}");
        }
    }
}