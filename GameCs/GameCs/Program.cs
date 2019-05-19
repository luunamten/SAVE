using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading;

namespace GameCs
{
    

    class Program
    {

        static CentraProccessing cpu;

        private static void keyEvent()
        {
            while (true)
            {
                char key = Console.ReadKey(true).KeyChar;
                cpu.OnPress(key);
            }

        }


        static void Main(string[] args)
        {
            
            cpu = new CentraProccessing();           
            keyEvent();
        }
    }
}
