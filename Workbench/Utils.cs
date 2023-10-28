using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    public static class Utils
    {
        public static void CopyDirectory(string sourceDir, string destinationDir, bool recursive, List<string>? filesToIgnore = null)
        {
            var dir = new DirectoryInfo(sourceDir);
            if (!dir.Exists)
            {
                Debug.WriteLine($"Source directory not found: {dir.FullName}");
                return;
            }

            DirectoryInfo[] dirs = dir.GetDirectories();

            if (!Directory.Exists(destinationDir))
            {
                Directory.CreateDirectory(destinationDir);
            }

            foreach(FileInfo file in dir.GetFiles())
            {
                if (filesToIgnore != null && filesToIgnore.Contains(file.Name))
                {
                    continue;
                }

                string targetFilePath = Path.Combine(destinationDir, file.Name);
                file.CopyTo(targetFilePath);
            }

            if (recursive)
            {
                foreach(DirectoryInfo subDir in dirs)
                {
                    string newDestinationDir = Path.Combine(destinationDir, subDir.Name);
                    CopyDirectory(subDir.FullName, newDestinationDir, true);
                }
            }
        }
    }
}
