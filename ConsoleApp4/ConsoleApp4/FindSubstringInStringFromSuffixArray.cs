using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp4
{
    public static class FindSubstringInStringFromSuffixArray
    {
        public static (int, int) Search(string Str, string SubStr)
        {
			// Ищем начальную позицию интервала
			int l = 0;
			int r = SubStr.Length;
			while (l < r)
            {
				int mid = (l + r) / 2;
				if (SubStr > suffixAt(A[mid]))
					l = mid + 1;
				else
					r = mid;

			}
			int s = l;



			return (0, 0);
        }
        public static int[] LCP(string str, string[] sufArray) 
        {
			int[] pos = new int[str.Length + 1];
			for (int i = 0; i <= str.Length; i++)
			{
				pos[i] = str.Length - sufArray[i].Length + 1;
			}
			int[] rank = new int[str.Length + 1];
			for (int i = 0; i <= str.Length; i++)
			{
				rank[pos[i]] = i;
			}
			int[] ans = new int[str.Length + 1];
			int h = 0;
			for (int i = 1; i <= str.Length; i++)
			{
				if (rank[i] > 1)
				{
					int k = pos[rank[i] - 1];
					while (((i + h - 1) != str.Length)
							&& ((k + h - 1) != str.Length)
							&& (str.ElementAt(i + h - 1) == str.ElementAt(k + h - 1)))
						h++;
					ans[rank[i]] = h;
					if (h > 0)
						h--;
				}
			}
			return ans; // ans[i + 1] = длина наибольшего общего префикса sufArray[i] и sufArray[i - 1]
		}
    }
}
