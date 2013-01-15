using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Windows.Controls;

namespace TimeInputSample.ViewModel
{
    public class TimeInputControlData : ViewModelBase
    {
        private int? hour_val;
        private int? minute_val;

        public int? Hour
        {
            get { return hour_val; }
            set
            {
                Debug.Assert(!value.HasValue || 0 <= value && value < 24);
                hour_val = value;
                NotifyPropertyChanged("HourText");
            }
        }

        public int? Minute
        {
            get { return minute_val; }
            set
            {
                Debug.Assert(!value.HasValue || 0 <= value && value < 60);
                minute_val = value;
                NotifyPropertyChanged("MinuteText");
            }
        }

        public string HourText
        {
            get { return hour_val.HasValue ? hour_val.Value.ToString() : null; }
            set
            {
                if (value == null || value == "")
                    hour_val = null;
                else
                {
                    int hour;
                    if (int.TryParse(value, out hour) && 0 <= hour && hour < 24)
                    {
                        hour_val = hour;
                        if (!minute_val.HasValue)
                        {
                            minute_val = 0;
                            NotifyPropertyChanged("MinuteText");
                        }
                    }
                }
                NotifyPropertyChanged("HourText");
            }
        }

        public string MinuteText
        {
            get { return minute_val.HasValue ? minute_val.Value.ToString("D2") : null; }
            set
            {
                if (value == null || value == "")
                    minute_val = null;
                else
                {
                    int minute;
                    if (int.TryParse(value, out minute) && 0 <= minute && minute < 60)
                        minute_val = minute;
                }
                NotifyPropertyChanged("MinuteText");
            }
        }

        public bool HasValue
        {
            get { return hour_val.HasValue && minute_val.HasValue; }
        }

        private CustomCommand got_focus_command;
        public CustomCommand GotFocusCommand
        {
            get
            {
                if (got_focus_command == null)
                    got_focus_command = new CustomCommand(OnGotFocus, CanGotFocus);

                return got_focus_command;
            }
        }

        public TimeInputControlData(int? hour, int? minute)
        {
            hour_val = hour;
            minute_val = minute;
        }

        public TimeInputControlData(DateTime? dt = null)
            : this(dt.HasValue ? dt.Value.Hour : null as int?, dt.HasValue ? dt.Value.Minute : null as int?) { }

        public DateTime? BlendWithDate(DateTime? date)
        {
            return HasValue && date.HasValue ? new DateTime(date.Value.Year, date.Value.Month, date.Value.Day,
                hour_val.Value, minute_val.Value, 0) : null as DateTime?;
        }

        private void OnGotFocus(object parameter)
        {
            var target = parameter as TextBox;
            target.SelectAll();
        }

        private bool CanGotFocus(object parameter)
        {
            return parameter is TextBox;
        }
    }
}
