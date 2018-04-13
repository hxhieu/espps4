<template>
  <div id="espps4">
    <router-view></router-view>
    <footer>
      <router-link to="/credits">CREDITS</router-link>
    </footer>
  </div>
</template>

<script>
import { mapActions } from 'vuex';
export default {
  name: 'espps4',
  created() {
    this.$http.getPayloads(result => {
      const payloads = [];

      result.forEach(p => {
        let key = Object.keys(p)[0];
        let name = key.replace(/\/payload_/gi, '').replace(/.js/gi, '');

        const fw = name.substr(0, 4);
        const sizeKb = p[key];
        const size = `${(sizeKb / 1024).toFixed(2)}Kb`;

        key = btoa(key);
        name = name.substring(5).toUpperCase();

        payloads.push({
          key,
          fw,
          name,
          sizeKb,
          size,
        });
      });

      this.storePayloads(payloads);
    });
  },
  methods: {
    ...mapActions(['storePayloads']),
  },
};
</script>

<style lang="less">
@primary-colour: #2c3e50;

.pull-left {
  float: left;
}

.clearfix:after {
  content: '';
  display: table;
  clear: both;
}

html,
body {
  font-family: Arial, Helvetica, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  color: @primary-colour;
  font-size: 30px;
  overflow: hidden;
  text-align: center;

  a.back {
    margin-left: 20px;
  }

  #espps4 {
    max-width: 1024px;
    margin: 20px auto;

    footer {
      position: fixed;
      bottom: 0;
      left: 0;
      right: 0;
      padding: 20px 0;
      text-align: center;
    }
  }

  .button {
    font-size: 1em;
    text-decoration: none;
    border: 2px solid @primary-colour;
    padding: 20px 40px;
    margin: 10px;
    transition: 0.25s all linear;
    color: @primary-colour;
    &:hover {
      background: @primary-colour;
      color: #fff;
    }
  }
  a {
    color: @primary-colour;
  }
}
</style>
