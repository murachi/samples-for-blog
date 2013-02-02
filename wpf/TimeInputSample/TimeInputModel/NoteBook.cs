using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Data;
using System.Data.SqlServerCe;

namespace TimeInputModel
{
    /// <summary>
    /// 日時メモ DB アクセス用オブジェクト
    /// </summary>
    /// <remarks>
    /// メモをたくさん収めたもの、ということで book などと洒落込んでますが、
    /// 実のところ、ただ単に一覧参照して、レコード追加できるだけの代物、みたいですよ。
    /// </remarks>
    public class NoteBook : IDisposable
    {
        /// <summary>
        /// 1レコード分の日時メモオブジェクト。
        /// </summary>
        /// <remarks>
        /// 一覧を取得するときにしか使わない、らしいですよ。
        /// </remarks>
        public class DateTimeNote
        {
            public int Code { get; private set; }
            public DateTime NoteDateTime { get; private set; }
            public string NoteDetail { get; private set; }

            internal DateTimeNote(int code, DateTime date_time, string detail)
            {
                Code = code;
                NoteDateTime = date_time;
                NoteDetail = detail;
            }
        }

        /// <summary>
        /// 日時メモ一覧オブジェクト。
        /// </summary>
        /// <remarks>
        /// 日時メモを DB から順次取り出すためのオブジェクト、らしいですよ。
        /// foreach 文を使って順次取り出せるようにしてある、みたいですよ。
        /// </remarks>
        public class NoteCollection : IEnumerable
        {
            SqlCeCommand sql_command;

            internal NoteCollection(SqlCeCommand command)
            {
                sql_command = command;
            }

            public IEnumerator GetEnumerator()
            {
                var reader = sql_command.ExecuteReader();

                while (reader.Read())
                {
                    var note = new DateTimeNote((int)reader["NOTE_CODE"],
                        (DateTime)reader["NOTE_DATETIME"], reader["NOTE_DETAIL"] as string);

                    yield return note;
                }
            }
        }

        SqlCeConnection connection = new SqlCeConnection("Data Source = DateTimeNotes.sdf");

        /// <summary>
        /// コンストラクタ。
        /// </summary>
        public NoteBook()
        {
            connection.Open();
        }

        /// <summary>
        /// ディスポーザ。
        /// </summary>
        /// <remarks>
        /// using ブロックを使えば、スコープから抜けた時点で DB が close する、らしいですよ。
        /// </remarks>
        public void Dispose()
        {
            connection.Close();
        }

        /// <summary>
        /// 日時メモ一覧アクセサ。
        /// </summary>
        public NoteCollection ActiveData
        {
            get
            {
                var command = connection.CreateCommand();
                command.CommandText = "select * from NOTES_TABLE";
                return new NoteCollection(command);
            }
        }

        /// <summary>
        /// 日時メモの記録を追加する。
        /// </summary>
        /// <param name="date_time">日時</param>
        /// <param name="detail">メモの内容</param>
        public void AppendNote(DateTime date_time, string detail)
        {
            var command = connection.CreateCommand();
            command.CommandText = "insert NOTES_TABLE (NOTE_DATETIME, NOTE_DETAIL) values (@date_time, @detail)";
            command.Parameters.Add("@date_time", date_time);
            command.Parameters.Add("@detail", detail);
            command.Prepare();
            command.ExecuteNonQuery();
        }
    }
}
