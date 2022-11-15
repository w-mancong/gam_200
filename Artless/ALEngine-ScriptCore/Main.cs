using System;
using System.Runtime.CompilerServices;

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

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern static void NativeLog(string str, int v);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern static void NativeLog();
    }
}