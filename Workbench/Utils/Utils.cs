﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows;

using Workbench.GadgetAPIStructs;

namespace Workbench
{
    public static class Utils
    {
        public const ulong InvalidGUID = 0;

        public static void CopyDirectory(string sourceDir, string destinationDir, bool recursive, List<string?>? filesToIgnore = null)
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

        private static readonly float NearZero = float.MinValue;
        public static bool IsNearZero(float s_) => s_ <= NearZero && s_ >= -NearZero;
        public static bool IsNearZero(double s_) => s_ <= NearZero && s_ >= -NearZero;
        public static bool Near(float a_, float b_) => IsNearZero(a_ - b_);
        public static bool Near(double a_, double b_) => IsNearZero(a_ - b_);

        public static bool Near(float? a_, float? b_)
        {
            if (a_ == null && b_ == null)
            {
                return true;
            }
            else if (a_ == null || b_ == null)
            {
                return false;
            }
            return Near(a_.Value, b_.Value);
        }

        public static bool Near(double? a_, double? b_)
        {
            if (a_ == null && b_ == null)
            {
                return true;
            }
            else if (a_ == null || b_ == null)
            {
                return false;
            }
            return Near(a_.Value, b_.Value);
        }

        public static int Contains(List<NamedVar> namedVars, string key)
        {
			//TODO
            return -1;
        }

        public static T? GetChildOfType<T>(this DependencyObject depObj, Predicate<T> predicate) where T : DependencyObject
        {
            if (depObj == null)
            {
                return null;
            }

            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(depObj); i++)
            {
                var child = VisualTreeHelper.GetChild(depObj, i);

                var result = (child as T) ?? GetChildOfType<T>(child, predicate);
                if (result != null && predicate.Invoke(result))
                {
                    return result;
                }
            }

            return null;
        }
    }
}
