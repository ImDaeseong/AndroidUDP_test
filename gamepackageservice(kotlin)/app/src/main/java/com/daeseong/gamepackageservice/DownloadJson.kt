package com.daeseong.gamepackageservice

import android.os.AsyncTask
import android.util.Log
import org.json.JSONArray

class DownloadJson : AsyncTask<Void?, Void?, String>() {

    private val tag = DownloadJson::class.java.simpleName

    //사용시에는 리얼 IP 필요
    private val sUrl = "http://127.0.0.1:8080/api/AllList"

    override fun doInBackground(vararg params: Void?): String? {

        var sResult = ""

        try {

            sResult = HttpUtil.GetGameDataResult(sUrl)
        } catch (ex: Exception) {

            Log.e(tag, ex.message.toString())
        }
        return sResult
    }

    override fun onPostExecute(sResult: String) {
        super.onPostExecute(sResult)

        try {

            val jsonArray = JSONArray(sResult)
            for (i in 0 until jsonArray.length()) {

                val jsonObject = jsonArray.getJSONObject(i)
                Log.e("$i - ", jsonObject.toString())

                val id = jsonObject.getString("id")
                val packagename = jsonObject.getString("packagename")
                val gametitle = jsonObject.getString("gametitle")
                val gamedesc = jsonObject.getString("gamedesc")
                iteminfo.getInstance().setGameItem(packagename)
            }
        } catch (ex: Exception) {

            Log.e(tag, ex.message.toString())
        }
    }

}