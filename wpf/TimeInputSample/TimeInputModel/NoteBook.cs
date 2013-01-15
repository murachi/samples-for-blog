using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Data;
using System.Data.SqlServerCe;

namespace TimeInputModel
{
    public class NoteBook : IDisposable
    {
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

        public NoteBook()
        {
            connection.Open();
        }

        public void Dispose()
        {
            connection.Close();
        }

        public NoteCollection ActiveData
        {
            get
            {
                var command = connection.CreateCommand();
                command.CommandText = "select * from NOTES_TABLE";
                return new NoteCollection(command);
            }
        }

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
