package com.lkq.fafu.baidu_map.Get_GPS;

/**
 * Created by alienware on 2018/3/16.
 */
public class Bean {
    /**
     * dataType : GPS
     * data : {"x":"119.230880","y":"26.083377"}
     */

    private String dataType;
    private DataBean data;

    public String getDataType() {
        return dataType;
    }

    public void setDataType(String dataType) {
        this.dataType = dataType;
    }

    public DataBean getData() {
        return data;
    }

    public void setData(DataBean data) {
        this.data = data;
    }

    public static class DataBean {
        /**
         * x : 119.230880
         * y : 26.083377
         */

        private String x;
        private String y;

        public String getX() {
            return x;
        }

        public void setX(String x) {
            this.x = x;
        }

        public String getY() {
            return y;
        }

        public void setY(String y) {
            this.y = y;
        }
    }
}
