<template>
  <div id="app">
    <router-view></router-view>
  </div>
</template>

<style lang="less">
html,
body {
  font-family: 'Roboto', Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  font-size: 40px;
  overflow: hidden;

  #app {
    max-width: 1024px;
    margin: 20px auto;
  }

  .mdc-button {
    height: auto;
  }
}
</style>
