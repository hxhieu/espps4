<template>
  <div class="home">
    <h1>Firmwares</h1>
    <div class="clearfix">
      <router-link v-for="fw in firmwares" :key="fw" class="button pull-left" :to="`/firmware/${fw}`">{{ fw }}</router-link>
    </div>
  </div>
</template>

<script>
import { mapState } from 'vuex';

export default {
  name: 'home',
  computed: {
    ...mapState(['payloads']),
    firmwares() {
      return this.payloads
        .map(p => p.fw)
        .reduce((x, y) => (x.includes(y) ? x : [...x, y]), []);
    },
  },
};
</script>
