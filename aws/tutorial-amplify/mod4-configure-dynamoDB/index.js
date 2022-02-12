// ※サンプルで let になっているものも const で良さそうなのは全部 const にしてます。

// AWS SDKモジュール
const AWS = require('aws-sdk');

// DynamoDB の document client とやらを使うらしい。
const dynamoDB = new AWS.DynamoDB.DocumentClient();
// サンプル通りだけどこれは悪しき習慣やな。本当は DB が持ってる時計を使うべき。
const now = new Date().toISOString();

// AWS lambda のハンドラ関数定義やで。
exports.handler = async (event) => {
    // イベントオブジェクトからパラメータ値が取れるのかな?
    const name = JSON.stringify(`やぁ、 ${event.firstName} ${event.lastName} 君。ボクが Lambda だよ。`);

    // DynamoDB テーブルに値を突っ込む
    const params = {
      TableName: 'myTutorialDatabase'
      , Item: {
        'ID': name
        , 'LatestGreetingTime': now
      }
    };
    await dynamoDB.put(params).promise();

    // レスポンスオブジェクトはこんな感じ。
    const response = {
        statusCode: 200,
        body: name
    };
    return response;
};
