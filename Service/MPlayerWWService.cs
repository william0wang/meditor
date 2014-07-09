using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.ServiceProcess;
using System.Text;

namespace MPlayerWWService
{
    public partial class IOService : ServiceBase
    {
        public IOService()
        {
            InitializeComponent();
        }

        void changeIOPrio(int pid, uint ioPrio)
        {
            int hProcess = Win32.OpenProcess(PROCESS_RIGHTS.PROCESS_ALL_ACCESS, false, pid);
            if (hProcess != 0)
                setIOPrio(hProcess, ioPrio);
        }

        unsafe void setIOPrio(int hProcess, uint newPrio)
        {
            uint ioPrio = newPrio;
            Win32.NtSetInformationProcess(hProcess, PROCESS_INFORMATION_CLASS.ProcessIoPriority,
                 (IntPtr)(&ioPrio), 4);
        }

        protected override void OnStart(string[] args)
        {
            if (args.Length > 0)
            {
                int pid = int.Parse(args[0]);
                if(pid > 0)
                    changeIOPrio(pid, 3);
            }

            Stop();
        }

        protected override void OnStop()
        {
        }


    }
}
