// AWS lambda のハンドラ関数定義やで。
exports.handler = async (event) => {
    // イベントオブジェクトからパラメータ値が取れるのかな?
    let name = JSON.stringify(`やぁ、 ${event.firstName} ${event.lastName} 君。ボクが Lambda だよ。`);

    // レスポンスオブジェクトはこんな感じ。
    const response = {
        statusCode: 200,
        body: name
    };
    return response;
};
