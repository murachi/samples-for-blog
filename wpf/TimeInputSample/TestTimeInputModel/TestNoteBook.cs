using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NUnit.Framework;

using TimeInputModel;

namespace TestTimeInputModel
{
    /// <summary>
    /// 日時メモ DB アクセスの単体テスト
    /// </summary>
    /// <remarks>
    /// NUnit を使っています。
    /// NUnit の GUI ツールで DLL を開いて実行すれば、テストが実行される、らしいですよ。
    /// </remarks>
    [TestFixture]
    public class TestNoteBook
    {
        /// <summary>
        /// 日時メモを書いたり読んだりするテスト
        /// </summary>
        /// <remarks>
        /// 一連の操作で例外が発生しないことを確認するぐらいの意味しかない、みたいですよ。
        /// </remarks>
        [Test]
        public void WriteAndRead()
        {
            DateTime now = DateTime.Now;
            using (var book = new NoteBook())
            {
                book.AppendNote(now, "日付メモ 記録テスト サンプルデータです…。");
            }

            using (var book = new NoteBook())
            {
                var data = book.ActiveData;
                bool is_success = false;
                foreach (NoteBook.DateTimeNote note in data)
                {
                    // SQL Server Compact の datetime 型は精度が 3.33ミリ秒と中途半端なので、
                    // 秒単位で一致すれば ok とする。
                    if (now.ToString("yyyMMddHHmmss") != note.NoteDateTime.ToString("yyyMMddHHmmss"))
                        continue;

                    is_success = true;
                    break;
                }

                Assert.IsTrue(is_success, "直前に書き込んだはずの日時のメモが見つからない。");
            }
        }
    }
}
