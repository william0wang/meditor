namespace MPlayerWWService
{
    partial class ProjectInstaller
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 组件设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.MPlayerWWServiceInstaller = new System.ServiceProcess.ServiceProcessInstaller();
            this.MPlayerWWService = new System.ServiceProcess.ServiceInstaller();
            // 
            // MPlayerWWServiceInstaller
            // 
            this.MPlayerWWServiceInstaller.Account = System.ServiceProcess.ServiceAccount.LocalSystem;
            this.MPlayerWWServiceInstaller.Installers.AddRange(new System.Configuration.Install.Installer[] {
            this.MPlayerWWService});
            this.MPlayerWWServiceInstaller.Password = null;
            this.MPlayerWWServiceInstaller.Username = null;
            // 
            // MPlayerWWService
            // 
            this.MPlayerWWService.Description = "MPlayer WW IO Service";
            this.MPlayerWWService.DisplayName = "MPlayerWWService";
            this.MPlayerWWService.ServiceName = "MPlayerWWService";
            this.MPlayerWWService.AfterInstall += new System.Configuration.Install.InstallEventHandler(this.serviceInstaller1_AfterInstall);
            // 
            // ProjectInstaller
            // 
            this.Installers.AddRange(new System.Configuration.Install.Installer[] {
            this.MPlayerWWServiceInstaller});

        }

        #endregion

        private System.ServiceProcess.ServiceProcessInstaller MPlayerWWServiceInstaller;
        private System.ServiceProcess.ServiceInstaller MPlayerWWService;
    }
}