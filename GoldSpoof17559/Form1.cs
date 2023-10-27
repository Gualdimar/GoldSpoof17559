using System;
using System.Drawing;
using System.Windows.Forms;

using XDevkit;
using JRPC_Client;

namespace GoldSpoof17559
{
    public partial class Form1 : Form
    {
        IXboxConsole Console;

        public Form1()
        {
            InitializeComponent();

        }

        private void button1_Click(object sender,EventArgs e)
        {
            if (Console.Connect(out Console,textBox1.Text))
            {
                button1.Text = "Re-Connect";
                label2.ForeColor = Color.Green;
                label2.Text = "Connected";
            }
            else
            {
                button1.Text = "Connect";
                label2.ForeColor = Color.Red;
                label2.Text = "Not connected";
            }
        }

        private void button3_Click(object sender,EventArgs e)
        {
            Console.SetMemory(0x816DAC84,new byte[] { 0x38,0x60,0x00,0x06 });    //XamUserGetMembershipTier
            Console.SetMemory(0x81A3CD60,new byte[] { 0x38,0x60,0x00,0x01 });

            Console.SetMemory(0x816DD688,new byte[] { 0x39,0x60,0x00,0x01 });    //XamUserCheckPrivilege
            Console.SetMemory(0x816DD6E8,new byte[] { 0x39,0x60,0x00,0x01 });    //XamUserCheckPrivilege
            Console.SetMemory(0x816DD6F4,new byte[] { 0x39,0x60,0x00,0x01 });    //XamUserCheckPrivilege
            Console.SetMemory(0x816DD6FC,new byte[] { 0x39,0x60,0x00,0x01 });    //XamUserCheckPrivilege

            Console.SetMemory(0x816DACE0,new byte[] { 0x38,0x60,0x00,0x06 });    //XamUserGetMembershipTierFromXUID

            Console.CallVoid(JRPC.ThreadType.Title,"xam.xex",656,34,0xFF,2,"You are golden!".ToWCHAR(),1);

            MessageBox.Show("Spoofed!");
        }
    }
}
