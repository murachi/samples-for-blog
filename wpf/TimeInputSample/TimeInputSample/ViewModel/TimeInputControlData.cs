using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Windows.Controls;

namespace TimeInputSample.ViewModel
{
    /// <summary>
    /// 時刻入力コントロール用のデータオブジェクトクラス。
    /// </summary>
    /// <remarks>
    /// TimeInputSample.View.TimeInputControl カスタムコントロールに
    /// バインドするデータの定義、らしいですよ。
    /// </remarks>
    public class TimeInputControlData : ViewModelBase
    {
        private int? hour_val;
        private int? minute_val;

        /// <summary>
        /// 時刻の時。
        /// </summary>
        /// <remarks>
        /// プログラム側から時刻の時の値にアクセスしたい場合用のアクセサ、らしいですよ。
        /// 時刻の時の部分が空欄の場合は null になる、みたいですよ。
        /// </remarks>
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

        /// <summary>
        /// 時刻の分。
        /// </summary>
        /// <remarks>
        /// プログラム側から時刻の分の値にアクセスしたい場合用のアクセサ、らしいですよ。
        /// 時刻の分の部分が空欄の場合は null になる、みたいですよ。
        /// </remarks>
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

        /// <summary>
        /// 時刻の時の文字列表現。
        /// </summary>
        /// <remarks>
        /// View の、時刻の時を表示する部分に、実際にバインドされるアクセサ、らしいですよ。
        /// ユーザーによる入力をチェックして、おかしな入力は受け付けないようになっている、みたいですよ。
        /// </remarks>
        public string HourText
        {
            get { return hour_val.HasValue ? hour_val.Value.ToString() : null; }
            set
            {
                if (value == null || value == "")
                {
                    Hour = null;
                    return;
                }
                int hour;
                if (int.TryParse(value, out hour) && 0 <= hour && hour < 24)
                {
                    Hour = hour;
                    if (!minute_val.HasValue)
                    {
                        Minute = 0;
                    }
                }
            }
        }

        /// <summary>
        /// 時刻の分の文字列表現。
        /// </summary>
        /// <remarks>
        /// View の、時刻の分を表示する部分に、実際にバインドされるアクセサ、らしいですよ。
        /// ユーザーによる入力をチェックして、おかしな入力は受け付けないようになっている、みたいですよ。
        /// </remarks>
        public string MinuteText
        {
            get { return minute_val.HasValue ? minute_val.Value.ToString("D2") : null; }
            set
            {
                if (value == null || value == "")
                {
                    Minute = null;
                    return;
                }
                int minute;
                if (int.TryParse(value, out minute) && 0 <= minute && minute < 60)
                    Minute = minute;
            }
        }

        /// <summary>
        /// 入力済みチェック。
        /// </summary>
        /// <remarks>
        /// 時刻がちゃんと入力されていれば真を返す、らしいですよ。
        /// </remarks>
        public bool HasValue
        {
            get { return hour_val.HasValue && minute_val.HasValue; }
        }

        private CustomCommand got_focus_command;
        /// <summary>
        /// フォーカス受付コマンド。
        /// </summary>
        /// <remarks>
        /// 時刻の時や分を表示する部分にフォーカスが移ったときに呼び出されるコマンドを取得するアクセサ、
        /// らしいですよ。ややこしいですね。
        /// よーするに、フォーカスが当たったら中身の文字列 (数字) を全選択にしたい、みたいですよ。
        /// </remarks>
        public CustomCommand GotFocusCommand
        {
            get
            {
                if (got_focus_command == null)
                    got_focus_command = new CustomCommand(OnGotFocus, CanGotFocus);

                return got_focus_command;
            }
        }

        /// <summary>
        /// コンストラクタ。
        /// </summary>
        /// <param name="hour">時刻の時。</param>
        /// <param name="minute">時刻の分。</param>
        public TimeInputControlData(int? hour, int? minute)
        {
            hour_val = hour;
            minute_val = minute;
        }

        /// <summary>
        /// コンストラクタ。
        /// </summary>
        /// <param name="dt">日時。</param>
        /// <remarks>
        /// 日時の時刻の部分だけを利用するコンストラクタ、らしいですよ。
        /// 引数を省略すると空欄になる、みたいですよ。
        /// </remarks>
        public TimeInputControlData(DateTime? dt = null)
            : this(dt.HasValue ? dt.Value.Hour : null as int?, dt.HasValue ? dt.Value.Minute : null as int?) { }

        /// <summary>
        /// 日付に時刻を加えて日時に変換する。
        /// </summary>
        /// <param name="date">日付。</param>
        /// <returns>日付と時刻を組み合わせた日時。</returns>
        /// <remarks>
        /// 日付も時刻も値が有効であれば、それらを組み合わせた日時の値を作ってくれるメソッド、らしいですよ。
        /// </remarks>
        public DateTime? BlendWithDate(DateTime? date)
        {
            return HasValue && date.HasValue ? new DateTime(date.Value.Year, date.Value.Month, date.Value.Day,
                hour_val.Value, minute_val.Value, 0) : null as DateTime?;
        }

        /// <summary>
        /// フォーカス受付コマンドの実際の処理。
        /// </summary>
        /// <param name="parameter">コマンドパラメータ。</param>
        /// <remarks>
        /// コマンドパラメータに、実際にフォーカスを受け付けたテキストボックスオブジェクトが入ってくることを
        /// 想定している、みたいですよ。
        /// </remarks>
        private void OnGotFocus(object parameter)
        {
            var target = parameter as TextBox;
            target.SelectAll();
        }

        /// <summary>
        /// フォーカス受付コマンド実行可否チェック。
        /// </summary>
        /// <param name="parameter">コマンドパラメータ。</param>
        /// <returns>コマンドが実行可能なら true を返す。</returns>
        /// <remarks>
        /// コマンドパラメータがテキストボックスオブジェクトかどうかをチェックしている、みたいですよ。
        /// </remarks>
        private bool CanGotFocus(object parameter)
        {
            return parameter is TextBox;
        }
    }
}
