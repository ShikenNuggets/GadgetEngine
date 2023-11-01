using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.IO.Packaging;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace Workbench
{
    public enum MessageType
    {
        Verbose = 0x01,
        Info = 0x02,
        Warning = 0x04,
        Error = 0x10,
    }

    public class LogMessage
    {
        public DateTime Time { get; }
        public MessageType MessageType { get; }
        public string Message { get; }
        public string File { get; }
        public string Caller { get; }
        public int Line { get; }

        public string MetaData => $"{File}: {Caller}: {MetaData}:";

        public LogMessage(MessageType type, string message, string file, string caller, int line)
        {
            Time = DateTime.Now;
            MessageType = type;
            Message = message;
            File = Path.GetFileName(file);
            Caller = caller;
            Line = line;
        }
    }

    public static class Logger
    {
        private static int _messageFilter = (int)(MessageType.Info | MessageType.Warning | MessageType.Error);
        private readonly static ObservableCollection<LogMessage> _messages = new ObservableCollection<LogMessage>();
        public static ReadOnlyObservableCollection<LogMessage> Messages { get; } = new ReadOnlyObservableCollection<LogMessage>(_messages);

        public static CollectionViewSource FilteredMessages { get; } = new CollectionViewSource() { Source = Messages };

        static Logger()
        {
            FilteredMessages.Filter += (s, e) =>
            {
                Debug.Assert(e != null && e.Item != null && e.Item is LogMessage);
                var type = (int)(e.Item as LogMessage).MessageType;
                e.Accepted = (type & _messageFilter) != 0;
            };
        }

        public static async void Log(MessageType type, string message, [CallerFilePath]string file = "", [CallerMemberName]string caller = "", [CallerLineNumber]int line = 0)
        {
            await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
            {
                _messages.Add(new LogMessage(type, message, file, caller, line));
            }));
        }

        public static async void Clear()
        {
            await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
            {
                _messages.Clear();
            }));
        }

        public static void SetMessageFilter(int mask)
        {
            _messageFilter = mask;
            FilteredMessages.View.Refresh();
        }
    }
}
