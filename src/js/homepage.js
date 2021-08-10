import Vue from "vue"

export default function setup_homepage() {
    let data = {
        data() {
            return {
                name: "World",
                count: 1,
                current_time: new Date().toLocaleString(),
                true_name: "Yes"
            }
        },
        mounted() {
            let that = this;
            setInterval(function () { that.current_time = new Date().toLocaleString() }, 1000);
        },
        methods: {
            increase() {
                this.count++;
            }
        }
    }


    Vue.createApp(data).mount('div')
}