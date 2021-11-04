<template>
  <Head/>
  <Body msg="Welcome to Your Vue.js App"/>
  <Foot/>
</template>

<script>
import Body from './components/Body.vue'
import Head from './components/Head.vue'
import Foot from './components/Foot.vue'

export default {
  name: 'App',
  components: {
    Head,
    Body,
    Foot
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}
</style>
