using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

using TimeInputModel;

namespace TimeInputSample.ViewModel
{
    /// <summary>
    /// メインウィンドウの View Model クラス。
    /// </summary>
    public class MainWindowViewModel : ViewModelBase
    {
        /// <summary>
        /// 日時メモ取得用オブジェクトクラス。
        /// </summary>
        /// <remarks>
        /// データグリッドに表示する日時メモ一覧のデータを順次取得するためのオブジェクトクラス、
        /// らしいですよ。
        /// </remarks>
        public class NoteListType : IEnumerable
        {
            /// <summary>
            /// 日時メモデータオブジェクトクラス。
            /// </summary>
            /// <remarks>
            /// データグリッドの 1行に対応するデータを定義するクラス、らしいですよ。
            /// </remarks>
            public class NoteData
            {
                private NoteBook.DateTimeNote dt_note;

                /// <summary>
                /// 日時列に表示する日時の文字列表現。
                /// </summary>
                public string NoteDateTime
                {
                    get { return dt_note.NoteDateTime.ToString("yyy/M/d H:mm"); }
                }

                /// <summary>
                /// メモ列に表示する内容。
                /// </summary>
                public string NoteDetail
                {
                    get { return dt_note.NoteDetail; }
                }

                /// <summary>
                /// コンストラクタ。
                /// </summary>
                /// <param name="note">元になるモデルデータ。</param>
                public NoteData(NoteBook.DateTimeNote note)
                {
                    dt_note = note;
                }
            }

            /// <summary>
            /// 反復子を取得するメソッド。
            /// </summary>
            /// <returns>DB 上の日時メモを NoteData に変換して順次返す反復子。</returns>
            /// <remarks>
            /// 詳しくは MSDN で IEnumerable を調べてみてくださいね。
            /// IEnumerable は foreach で回して値を順次取得もしくは生成するような
            /// オブジェクトを作りたい場合に実装するインタフェースですが、
            /// データグリッドなどのアイテムとしてデータバインドするのにも使えちゃう、らしいですよ。
            /// </remarks>
            public IEnumerator GetEnumerator()
            {
                using (var book = new NoteBook())
                {
                    foreach (NoteBook.DateTimeNote note in book.ActiveData)
                        yield return new NoteData(note);
                }
            }
        }

        /// <summary>
        /// 日時入力コントロールの値。
        /// </summary>
        public DateTimeInputControlData NoteDateTime { get; private set; }

        private string note_detail;
        /// <summary>
        /// メモの入力値。
        /// </summary>
        public string NoteDetail
        {
            get { return note_detail; }
            set
            {
                note_detail = value;
                NotifyPropertyChanged("NoteDetail");
            }
        }

        private CustomCommand write_command = null;
        /// <summary>
        /// 「書き込む」ボタンに割り当てるコマンド。
        /// </summary>
        public CustomCommand WriteCommand
        {
            get
            {
                if (write_command == null)
                    write_command = new CustomCommand(OnWrite, CanWrite);

                return write_command;
            }
        }

        private NoteListType date_time_notes;
        /// <summary>
        /// 日時メモ一覧データ。
        /// </summary>
        public NoteListType DateTimeNotes
        {
            get { return date_time_notes; }
            private set
            {
                date_time_notes = value;
                NotifyPropertyChanged("DateTimeNotes");
            }
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public MainWindowViewModel()
        {
            NoteDateTime = new DateTimeInputControlData();
            date_time_notes = new NoteListType();
        }

        /// <summary>
        /// 「書き込む」ボタン実行時処理。
        /// </summary>
        /// <param name="parameter">コマンドパラメータ。常に null。</param>
        /// <remarks>
        /// 入力された日時とメモを DB に書き込んだ後、 DB テーブルの新たな内容を
        /// 取得するためのデータオブジェクトを生成し直してデータグリッドに反映する
        /// ということをやっている、らしいですよ。
        /// </remarks>
        private void OnWrite(object parameter)
        {
            using (var book = new NoteBook())
            {
                book.AppendNote(NoteDateTime.DateTimeValue.Value, note_detail);
            }
            DateTimeNotes = new NoteListType();
        }

        /// <summary>
        /// 「書き込む」ボタン実行可否判定。
        /// </summary>
        /// <param name="parameter">コマンドパラメータ。常に null。</param>
        /// <returns>実行可能なら true を返す。</returns>
        /// <remarks>
        /// 日時が入力されていれば実行可能と判定する、らしいですよ。
        /// </remarks>
        private bool CanWrite(object parameter)
        {
            return NoteDateTime.HasValue;
        }
    }
}
