using System;
using System.Text;
using System.IO;
using System.Collections;

namespace RTParser
{
	class Parser {
		[STAThread]
		static void Main(string[] args) {
			ArrayList files = new ArrayList();
			string[] fs;
			files.AddRange(fs = Directory.GetFiles("H:\\q4x\\materials", "*.mtr"));
			files.AddRange(fs = Directory.GetFiles("H:\\q4x\\def", "*.def"));
			files.AddRange(fs = Directory.GetFiles("H:\\q4x\\skins", "*.skin"));
			files.AddRange(fs = Directory.GetFiles("H:\\q4x\\af", "*.af"));
			files.AddRange(fs = Directory.GetFiles("H:\\q4x\\sound", "*.sndshd"));

			foreach (string filename in files) {
				new Parser().run(filename);
			}
		}

		void run(string filename) {
			
		}
	}
}
