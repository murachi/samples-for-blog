using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TimeInputSample.ViewModel
{
    /// <summary>
    /// 日時入力コントロール用のデータオブジェクトクラス。
    /// </summary>
    /// <remarks>
    /// TimeInputSample.View.DateTimeInputControl カスタムコントロールに
    /// バインドするデータの定義、らしいですよ。
    /// </remarks>
    public class DateTimeInputControlData : ViewModelBase
    {
        private DateTime? date_val;

        /// <summary>
        /// 日付
        /// </summary>
        /// <remarks>
        /// View の DatePicker との値のやりとりに使うアクセサ、らしいですよ。
        /// 日時が欲しい場合は、 DateTimeValue を使いましょうね。
        /// </remarks>
        public DateTime? Date
        {
            get { return date_val; }
            set
            {
                date_val = value;
                NotifyPropertyChanged("Date");
            }
        }

        /// <summary>
        /// 時刻。
        /// </summary>
        /// <remarks>
        /// 時刻入力コントロール用のデータオブジェクトを返しますよ。
        /// </remarks>
        public TimeInputControlData Time { get; private set; }

        /// <summary>
        /// 日時。
        /// </summary>
        /// <remarks>
        /// プログラム側から日時の値にアクセスしたい場合用のアクセサ、らしいですよ。
        /// 日付も時刻もきっちり入力されている場合だけ、有効な値を返す、らしいですよ。
        /// それ以外の場合は、 null が返されちゃう、みたいですよ。
        /// </remarks>
        public DateTime? DateTimeValue
        {
            get { return Time.BlendWithDate(date_val); }
            set
            {
                setDateTime(value);
                NotifyPropertyChanged("Date");
                NotifyPropertyChanged("Time");
            }
        }

        /// <summary>
        /// 値入力済み判定。
        /// </summary>
        /// <remarks>
        /// 日付も時刻もきっちり入力されている場合は true を返す、らしいですよ。
        /// </remarks>
        public bool HasValue
        {
            get { return date_val.HasValue && Time.HasValue; }
        }

        /// <summary>
        /// コンストラクタ。
        /// </summary>
        /// <param name="dt">デフォルトの日時。</param>
        /// <remarks>
        /// デフォルトの値を空にしたい場合は、 dt に null を指定してくださいね。
        /// </remarks>
        public DateTimeInputControlData(DateTime? dt = null)
        {
            Time = new TimeInputControlData();
            setDateTime(dt);
        }

        private void setDateTime(DateTime? dt)
        {
            date_val = dt.HasValue ? dt.Value.Date : null as DateTime?;
            Time.Hour = dt.HasValue ? dt.Value.Hour : null as int?;
            Time.Minute = dt.HasValue ? dt.Value.Minute : null as int?;
        }
    }
}
