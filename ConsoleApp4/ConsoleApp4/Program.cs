using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp4
{
    class Program
    {
        private static String Charset = "abcdefghijklmnopqrstuvwxyz";

        /// <summary>
        /// Start Brute Force.
        /// </summary>
        /// <param name="length">Words length.</param>
        public static void StartBruteForce(int length)
        {
            StringBuilder sb = new StringBuilder(length);
            char currentChar = Charset[0];

            for (int i = 1; i <= length; i++)
            {
                sb.Append(currentChar);
            }

            int counter = 0;
            ChangeCharacters(0, sb, length, ref counter);
            Console.WriteLine(counter);
        }

        private static void ChangeCharacters(int pos, StringBuilder sb, int length, ref int counter)
        {
            for (int i = 0; i <= Charset.Length - 1; i++)
            {
                sb[pos] = Charset[i];
                if (pos == length - 1)
                {
                    counter++;
                    Console.WriteLine(sb.ToString());
                }
                else
                {
                    ChangeCharacters(pos + 1, sb, length, ref counter);
                }
            }
        }


            static void Main(string[] args)
        {
            StartBruteForce(3);
        }
    }
}
