using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Workbench
{
    public static class Serializer
    {
        public static void ToFile<T>(T obj, string path) where T : class{
            try
            {
                using var fs = XmlWriter.Create(path, new XmlWriterSettings { Indent = true });
                var serializer = new DataContractSerializer(typeof(T));
                serializer.WriteObject(fs, obj);
            }
            catch(Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, $"Failed to serialize {obj} to {path}!");
                throw;
            }
        }

        public static T? FromFile<T>(string path)
        {
            try
            {
                using var fs = new FileStream(path, FileMode.Open);
                var serializer = new DataContractSerializer(typeof(T));
                var serializedObject = serializer.ReadObject(fs);
                if (serializedObject != null)
                {
                    return (T)serializedObject;
                }
                else
                {
                    return default;
                }
            }
            catch(Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, $"Failed to deserialize from {path}!");
                throw;
            }
        }
    }
}
