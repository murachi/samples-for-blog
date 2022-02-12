# 基本的なウェブアプリケーションを構築するチュートリアルをやってみた

https://aws.amazon.com/jp/getting-started/hands-on/build-web-app-s3-lambda-api-gateway-dynamodb/

## 実装: Amplify コンソールでウェブアプリを作成
- [mod1-build-amplify-app ディレクトリへ](mod1-build-amplify-app/)

> 3. HTML ファイルのみを ZIP (圧縮) します。

アーカイバは何使ってもいいのか? とりあえず 7zip 形式を使ってみる。

> 6. [Git プロバイダーなしでデプロイ] を選択します。以下のように画面に表示されるはずです。

UI 全然違った(´・\_・\`)。
![Amplify 開始の実際の UI…。](mod1-build-amplify-app/figs/fig-ui-start-amplify.png)

Git 使わない方法だからとりあえず `Amplify Studio` を選んでおけばいいのかな?

> 8. [App 名] フィールドに、「GettingStarted」と入力します。

`build-amplify-app-tutorial` という名前にしてみた。

バックエンド環境の構築はまあまあ時間がかかる様子… 1〜2分程度で完了。

> 9. 環境名には、「dev」と入力します。

どこに入力するんだ? (´・\_・\`) つか、すでに staging になっちゃってるっぽいが? (´・\_・\`)
![Amplify バックエンド構築直後の UI](mod1-build-amplify-app/figs/fig-ui-amplify-after-build-backend.png)

...わからんので一旦削除して最初からやり直してみることにした。
![一旦削除](mod1-build-amplify-app/figs/fig-delete-amplify-app.png)

「使用を開始する」の画面に戻ったので、今度は `Amplify Hosting` を選んでみた。

チュートリアルどおりの UI だー。こっちでよかったのか…
![チュートリアルどおりの UI が出てきた](mod1-build-amplify-app/figs/fig-ui-start-with-amplify-hosting.png)

で、ドラッグ&amp;ドロップで `.7z` のファイルを放り込んだらこうなりました。どうやらサポートしてないらしい…(´・\_・\`)
![7zは駄目でした…](mod1-build-amplify-app/figs/fig-7z-not-supported.png)

`.tar.xz` も試してみたが駄目だったので、仕方なく `.zip` にして放り込む (`.zip` はファイル名 Unicode に対応してないから避けたいんだけどね…)。で、「保存してデプロイ」でやっと先に進みました。

やり方さえ分かっちゃえば静的コンテンツを公開する方法としてはめっちゃ簡単だ罠。

## 実装: Lambda 関数を作成して設定
- [mod2-create-lambda-function ディレクトリへ](mod2-create-lambda-function/)

> 2. 必ず、関数を作成しているリージョンをメモしてください。これは、ページの最上部、アカウント名の横にあります。

オレゴンでした。

> 4. [関数名] で、「HelloWorldFunction」と入力します。

`myTutorialFunction` としてみました。

> 5. [ランタイム] ドロップダウンから [Node.js 12.x] を選択します。

最新に合わせて `Node.js 14.x` を選択。

![Lambda の基本的な設定](mod2-create-lambda-function/figs/fig-lambda-base-settings.png)

> 9. 画面上部にあるオレンジの [保存] ボタンをクリックします。

オレンジのボタンは「Test」でした。そして先に「Deploy」ボタンを押すのが正解だったっぽい?

![コード入力枠の上にあるオレンジのボタン…](mod2-create-lambda-function/figs/fig-orange-button-on-code-area.png)

実際先に「Test」を押すと、テスト用の Lambda 関数を設定するフォームが出てきて、関数名とテスト用のパラメータ (request body となる JSON) の設定を要求される。関数名は同じく `myTutorialFunction` としてみた。(このへん画像を取り損なった…)

ただそれだけだと何故かデフォルトの Hello world 関数みたいなのが設定されちゃうので、ここで「Deploy」をクリックすると、先に設定したテスト用 Lambda 関数にコードの内容がデプロイされて、やっと「Test」ボタンでテストできるようになる。

![Test を実行したところ](mod2-create-lambda-function/figs/fig-lambda-test-executed.png)

## サーバレス関数にリンクする
- [mod3-create-REST-API ディレクトリへ](mod3-create-REST-API/)

まず始める前にリージョンを確認したほうが良さげ。 API Gateway のページに入った段階で、リージョンの設定が↑と違うなら選択し直す。わい、ここでバージニア北部になってるのに気づかずに進めちゃってめっさ手戻りした… orz

![リージョンはちゃんと確認しよう](mod3-create-REST-API/figs/fig-api-gateway-region.png)

### 実装: 新しい REST API を作成する
> 2. オレンジの [API の作成] ボタンをクリックします。
> 3. REST API ボックスを見つけて、その中のオレンジの [構築] ボタンをクリックします。

2 は無かった。3 はこれのことかな?

![API Gateway の最初のページ、の左下にあったブロック](mod3-create-REST-API/figs/fig-api-gateway-1st-page.png)

なんかアラートが出た。インポートフォーム? にサンプルプログラムをとりあえず突っ込んどいたって話?

![のっけからよくわからんアラート](mod3-create-REST-API/figs/fig-api-gateway-1st-alert.png)

> 4. [プロトコルの選択] で、[REST] を選択します。
> 4. [Create new API] で、[New API] を選択します。
> 4. [API 名] フィールドに、「HelloWorldAPI」と入力します。
> 4. [エンドポイントタイプ] プルダウンで [エッジ最適化] を選択します

API名は `myTutorialAPI` としてみた。

![API作成時の設定内容](mod3-create-REST-API/figs/fig-api-gateway-create-setting.png)

### 実装: 新しいリソースとメソッドを作成する
> 2. [/] リソースを選択した状態で、[アクション] ドロップダウンメニューから [メソッドの作成] をクリックします。

これか。

![「アクション」ドロップダウンメニューの「メソッドの作成」](mod3-create-REST-API/figs/fig-api-gateway-create-method.png)

> 3. 表示される新しいドロップダウンから POST を選択し、チェックマークをクリックします。

これのことらしい。(ドロップ開いた状態ではキャプチャが上手く取れなかった…)

![HTTPメソッドの選択](mod3-create-REST-API/figs/fig-api-gateway-method-http-method.png)

> 4. 統合タイプとして [Lambda 関数] を選択します。
> 4. [関数] フィールドに、「HelloWorldFunction」と入力します。
> 4. ブルーの [保存] ボタンをクリックします。

関数フィールドに、前のチュートリアルで作った関数名 `myTutorialFunction` を入力しようとすると、ちゃんと保管候補が出てくる。入力後の画面はこんな感じ。

![リソースの設定内容](mod3-create-REST-API/figs/fig-api-gateway-resource-setting.png)

> 7. 作成している API に Lambda 関数を呼び出すアクセス許可を与えていることを知らせるメッセージが表示されます。[OK] ボタンをクリックします。

Lambda 関数に権限をつけちゃうよ、というアラートが出た。

![リソースの設定内容](mod3-create-REST-API/figs/fig-api-gateway-alert-authority.png)

> 8. 新しく作成した POST メソッドを選択した状態で、[アクション] ドロップダウンメニューから [CORS を有効にする] を選択します。

これか。

![CORS を有効に](mod3-create-REST-API/figs/fig-api-gateway-activate-CORS.png)

あとはだいたい書いてある通り。

### 実装: API をデプロイする

> 1. [アクション] ドロップダウンリストで、[Deploy API] を選択します。

これですね。

![Deploy API メニュー](mod3-create-REST-API/figs/fig-api-gateway-api-deploy-menu.png)

> 2. [デプロイステージ] ドロップダウンリストから [新しいステージ] を選択します。
> 2. [ステージ名] に「dev」と入力します。

こんな感じかな。

![「APIのデプロイ」ダイアログの設定内容](mod3-create-REST-API/figs/fig-api-gateway-api-deploy-setting.png)

> 5. [URL を呼び出す] の横にある URL をコピーして保存します (モジュール 5 で必要になります)。

こいつのことらしい。

![URLの呼び出し](mod3-create-REST-API/figs/fig-api-gateway-API-URL.png)

URL:
```
https://8ks7kzg0b7.execute-api.us-west-2.amazonaws.com/dev
```

## データテーブルを作成する
- [mod4-configure-dynamoDB ディレクトリへ](mod4-configure-dynamoDB/)

まずはリージョンを確認しましょう(´・\_・\`) 今度は何故かデフォがオハイオだった(´・\_・\`)

### 実装: DynamoDB テーブルを作成する

> 3. ブルーの [テーブルの作成] ボタンをクリックします。

オレンジなんだが…(´・\_・\`)

![テーブルの作成ボタン](mod4-configure-dynamoDB/figs/fig-dynamo-create-table-button.png)

> 4. [テーブル名] の横に、HelloWorldDatabase と入力します。
> 4. [プライマリキー] フィールドに、ID と入力します。

テーブル名は `myTutorialDatabase` としてみた。「プライマリキー」は、今は「パーティションキー」と呼んでいるらしい。

![設定内容](mod4-configure-dynamoDB/figs/fig-dynamo-create-table-settings.png)

> 6. ブルーの [作成] ボタンをクリックします。

だからオレンジだっつの…

![作成ボタン](mod4-configure-dynamoDB/figs/fig-dynamo-create-table-decide-button.png)

状態は作成中だが正常に作成されたらしい。つかなんで 2行あるんだ? (´・\_・\`)

![作成を実行後の画面](mod4-configure-dynamoDB/figs/fig-dynamo-create-table-fixed.png)

> 7. 右側のパネルからテーブルの「Amazon リソースネーム (ARN) 」をコピーします (後でこのモジュールで必要になります)。

作成後の画面が↑の状態だったので ARN なんて出てこねーぞって最初は焦った(´・\_・\`)

ブラウザを更新したらこんな感じの画面になったので、

![ブラウザの更新を実行後](mod4-configure-dynamoDB/figs/fig-dynamo-reload-after-table-created.png)

テーブル名をクリックして設定画面を開いたら、「一般的な情報」セクションの「追加情報」をクリックすると…

![テーブルの設定ページ](mod4-configure-dynamoDB/figs/fig-dynamo-table-setting-page.png)

同セクションの下の方に ARN が表示された。

![ARNめっけた](mod4-configure-dynamoDB/figs/fig-dynamo-ARN.png)

ARN:
```
arn:aws:dynamodb:us-west-2:157489307820:table/myTutorialDatabase
```

### 実装: IAMポリシーを作成してLambda関数に追加

> 3. DynamoDB サービスを使用できるように関数にアクセス許可を追加します。このために、AWS Identity and Access Management (IAM) を使用します。
> 3. [アクセス許可] タブをクリックします。
> 3. [実行ロール] ボックスで、ロールをクリックします。新しいブラウザタブが開きます。

3 はただの説明? 4 は全然違っていて、「設定」タブの「アクセス権限」メニューが正解。

![ロールの在り処](mod4-configure-dynamoDB/figs/fig-lambda-role.png)

> 6. [アクセス許可ポリシー] ボックスの右側にある [インラインポリシーの追加] をクリックします。

「許可ポリシー」セクション右側の「アクセス許可を追加」→「インラインポリシーを作成」が正解。

![ロールのインラインポリシー設定メニュー](mod4-configure-dynamoDB/figs/fig-lambda-role-create-inline-policy-menu.png)

> 12. ブルー の [ポリシーの作成] ボタンをクリックします。
> 12. これで、このブラウザータブを閉じて、Lambda 関数のタブに戻ることができます。

気が早いな…「ポリシーの作成」ボタンを押したらこんなページに遷移したぞ。

![ポリシーの確認ページ](mod4-configure-dynamoDB/figs/fig-lambda-role-policy-confirm.png)

名前が必須らしいので、 `myTutorialFunctionRolePolicy` としてみた。

で、画面最下部にまだ「ポリシーの作成」ボタンが居るので、もう一度クリック。これでようやくポリシーの設定が完了。今作成したポリシーがちゃんと追加されていることが確認できる。

![作成したロールが追加されていることを確認](mod4-configure-dynamoDB/figs/fig-lambda-role-create-fixed.png)

### 実装: Lambda 関数を変更して DynamoDB テーブルに書き込む

> 1. [設定] タブをクリックします。

相変わらずおかしなことを言ってるが、ここは「コード」タブやな。(面倒なので画像は割愛)

> 3. ページの上部にあるオレンジの [保存] ボタンをクリックします。

これもオレンジの「Test」ボタンで動作確認してからの、白い「Deploy」ボタンが正解やな。

### 実装: 変更をテスト

> 1. 白の [テスト] ボタンをクリックします。
> 1. グリーンの背景の「実行結果: 成功」メッセージが表示されます。

DynamoDB への書き込みを実際に行うテストを駆動したい場合は、コードタブ内の「テスト」ボタンではなく、「テスト」タブを使うのが正しいらしい。こちらの「テストイベント」セクション内にあるオレンジの「テスト」ボタンをクリックすると、下の画像のような結果の画面が得られる。

![テスト実行結果画面](mod4-configure-dynamoDB/figs/fig-lambda-test-result.png)

> 6. 右側の [アイテム] タブをクリックします。
> 6. テストイベントに一致するアイテムがここに表示されます。

オレンジの「テーブルアイテムの探索」ボタンをクリック、が正解。

![テーブルアイテムの探索ボタン](mod4-configure-dynamoDB/figs/fig-dynamo-find-items-button.png)

「アイテムのスキャン/クエリ」セクションで「スキャン」を選んでフィルター何もなしで「実行する」をクリックすれば、追加された値が閲覧できる。

![テーブルのアイテムをスキャン](mod4-configure-dynamoDB/figs/fig-dynamo-scan-items.png)
