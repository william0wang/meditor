using System;
using System.Collections.Generic;
using System.Text;
using System.ServiceProcess;
using System.Configuration.Install;
using System.Collections;

namespace MPlayerWWServiceStarter
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 1)
                return;

            StartService(args);
        }


        public static void InstallService()
        {
            string exePath = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            string strDir = System.IO.Path.GetDirectoryName(exePath);
            string filepath = strDir + "\\MPlayerWWService.exe";

            IDictionary mSavedState = new Hashtable();
            AssemblyInstaller myAssemblyInstaller = new AssemblyInstaller();
            myAssemblyInstaller.UseNewContext = true;
            myAssemblyInstaller.Path = filepath;
            myAssemblyInstaller.Install(mSavedState);
            myAssemblyInstaller.Commit(mSavedState);
            myAssemblyInstaller.Dispose();
        }

        public static void StartService(string[] args)
        {
            string serviceName = "MPlayerWWService";
            try
            {
                ServiceController[] services = ServiceController.GetServices();
                foreach (ServiceController s in services)
                {
                    if (s.ServiceName == serviceName)
                    {
                        s.Start(args);
                        return;
                    }
                }

                InstallService();

                ServiceController service = new ServiceController(serviceName);
                service.Start(args);
            }
            catch
            {
            }
        }
    }
}
