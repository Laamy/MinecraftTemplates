using System;
using System.Windows.Forms;

class Program
{
    [STAThread]
    static void Main(string[] args)
    {
        OpenFileDialog dialog = new OpenFileDialog()
        {
            Title = "Select the injectable DLL",
            DefaultExt = "dll",
            Filter = "dll files (*.dll)|*.dll|All files (*.*)|*.*",

            CheckFileExists = true,
            CheckPathExists = true
        };

        if (dialog.ShowDialog() == DialogResult.OK)
        {
            Console.WriteLine($"Injectng {dialog.FileName}");

            if (!TemplatePipe.InjectDLL(dialog.FileName))
            {
                Console.WriteLine("Injection failed!");
            }
            else Console.WriteLine("Injected successfully");
        }

        Console.ReadKey();
    }
}