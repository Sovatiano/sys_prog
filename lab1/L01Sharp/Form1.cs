using System.Diagnostics;

namespace L01Sharp
{
    public partial class Form1 : Form
    {
        Process childProcess = null;
        System.Threading.EventWaitHandle stopEvent = new EventWaitHandle(false, EventResetMode.ManualReset, "StopEvent");
        System.Threading.EventWaitHandle startEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        System.Threading.EventWaitHandle confirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        System.Threading.EventWaitHandle closeEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseEvent");

        int n = 0;
        int threadcount = 0;

        public Form1()
        {
            InitializeComponent();
            this.FormClosing += new FormClosingEventHandler(Form1_FormClosing);

        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (childProcess != null && !childProcess.HasExited)
            {

                closeEvent.Set();
                

            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                childProcess = Process.Start("L01.exe");
                threadcount = 0;
                listBox1.Items.Clear();
            }
            else
            {
                for (int i = 0; i < n; i++)
                {
                    startEvent.Set();
                    confirmEvent.WaitOne();
                    threadcount++;
                    listBox1.Items.Add("Thread " + threadcount.ToString());
                }

            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (!(childProcess == null || childProcess.HasExited))
            {
                stopEvent.Set();
                confirmEvent.WaitOne();
                threadcount--;
                listBox1.Items.Remove("Thread " + (threadcount + 1).ToString());
            }
            
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            decimal value = numericUpDown1.Value;
            n = Convert.ToInt32(value);
        }

        
    }
}
