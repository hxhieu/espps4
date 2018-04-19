<template>
  <div id="espps4">
    <router-view class="main"></router-view>
    <section class="menu">
      <router-link to="/">HOME</router-link>
      <router-link to="/payloads/4.55">PAYLOADS</router-link>
      <router-link to="/credits">CREDITS</router-link>
      <router-link to="/settings">SETTINGS</router-link>
    </section>
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
@import './variables';

.pull-left {
  float: left;
}

.clearfix:after {
  content: '';
  display: table;
  clear: both;
}

.hints {
  font-size: 0.65em;
  display: inline-block;
  color: #eee;
  margin-top: 10px;
}

html,
body {
  margin: 0;
  padding: 0;
  font-family: Arial, Helvetica, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  color: @primary-colour;
  font-size: 30px;
  text-align: center;
  overflow-y: scroll;
  text-shadow: 1px 1px 2px rgba(50, 50, 50, 0.5);
  background: rgba(255, 255, 255, 1);
  background: -moz-linear-gradient(
    left,
    rgba(255, 255, 255, 1) 0%,
    rgba(246, 246, 246, 1) 47%,
    rgba(237, 237, 237, 1) 100%
  );
  background: -webkit-gradient(
    left top,
    right top,
    color-stop(0%, rgba(255, 255, 255, 1)),
    color-stop(47%, rgba(246, 246, 246, 1)),
    color-stop(100%, rgba(237, 237, 237, 1))
  );
  background: -webkit-linear-gradient(
    left,
    rgba(255, 255, 255, 1) 0%,
    rgba(246, 246, 246, 1) 47%,
    rgba(237, 237, 237, 1) 100%
  );
  background: -o-linear-gradient(
    left,
    rgba(255, 255, 255, 1) 0%,
    rgba(246, 246, 246, 1) 47%,
    rgba(237, 237, 237, 1) 100%
  );
  background: -ms-linear-gradient(
    left,
    rgba(255, 255, 255, 1) 0%,
    rgba(246, 246, 246, 1) 47%,
    rgba(237, 237, 237, 1) 100%
  );
  background: linear-gradient(
    to right,
    rgba(255, 255, 255, 1) 0%,
    rgba(246, 246, 246, 1) 47%,
    rgba(237, 237, 237, 1) 100%
  );
  filter: progid:DXImageTransform.Microsoft.gradient(
      startColorstr='#ffffff',
      endColorstr='#ededed',
      GradientType=1
    );

  a.back {
    margin-left: 20px;
  }

  #espps4 {
    max-width: 1024px;
    margin: 0 auto;

    .main {
      padding: 20px 0;
    }

    .menu {
      position: fixed;
      top: 0;
      left: 0;
      right: 0;
      text-align: center;
      a {
        display: inline-block;
        margin: 10px 20px;
        font-size: 0.5em;
        font-weight: bold;
      }
      background: #fff;
    }
  }

  .button {
    font-size: 1em;
    text-decoration: none;
    border: 2px solid @primary-colour;
    padding: 20px 40px;
    margin: 10px;
    transition: @hover-transition;
    color: @primary-colour;
    display: inline-block;
    &:hover {
      background: @primary-colour;
      color: #fff;
    }
  }
  a {
    color: @secondary-colour;
    transition: @hover-transition;
    &:hover {
      color: @primary-colour;
    }
  }
}
</style>
