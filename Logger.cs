using System;
using System.IO;
using System.Text;

namespace Server
{
    internal class Logger : TextWriter
    {
        private readonly TextWriter[] writers;

        public Logger(params TextWriter[] writers)
        {
            this.writers = writers;
        }

        public override void Write(char value)
        {
            foreach (var writer in writers)
                writer.Write(value);
        }

        public override void WriteLine(string? value)
        {
            foreach (var writer in writers)
            {
                writer.Write($"[{DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff")}]: ");
                writer.WriteLine(value);
            }
        }

        public override Encoding Encoding => Encoding.UTF8;
    }
}
