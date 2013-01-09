using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Data;
using System.Data.SqlServerCe;

namespace TimeInputModel
{
    public class NoteBook
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

        private NoteBook()
        {
            connection.Open();
        }

        private NoteCollection ActiveData
        {
            get
            {
                var command = connection.CreateCommand();
                command.CommandText = "select * from NOTES_TABLE";
                return new NoteCollection(command);
            }
        }

        private void Insert(DateTimeNote note)
        {
        }

        private static NoteBook singleton = new NoteBook();

        public static NoteCollection GetActiveData()
        {
            return singleton.ActiveData;
        }

        public static int InsertNote(DateTime date_time, string detail)
        {

        }
    }
}
